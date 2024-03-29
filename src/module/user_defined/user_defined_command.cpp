#include "module/user_defined/user_defined_command.h"
#include <Windows.h>
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/range.h>
#include <zaf/base/string/join.h>
#include <zaf/creation.h>
#include "environment_variable_manager.h"
#include "help/help_content_building.h"
#include "help/markdown/element/factory.h"
#include "module/active_path/active_path_modifying.h"
#include "module/active_path/active_path_option_parsing.h"
#include "module/user_defined/parse/entry_command_parsing.h"
#include "module/user_defined/parse/variable_formatter.h"
#include "module/user_defined/preview/user_defined_command_preview_control.h"

using namespace ra::help::markdown::element;

namespace ra::mod::user_defined {
namespace {

class UserDefinedCommandExecutor : public CommandExecutor {
public:
    explicit UserDefinedCommandExecutor(std::shared_ptr<Entry> entry, ExecuteInfo execute_info) :
        entry_(std::move(entry)),
        execute_info_(std::move(execute_info)) {

    }

    void Execute() override {

        //Update current process' environment variables in order to inherit them in child process.
        EnvironmentVariableManager::Instance().Update();

        SHELLEXECUTEINFO shell_execute_info{};
        shell_execute_info.cbSize = sizeof(shell_execute_info);
        shell_execute_info.fMask = SEE_MASK_DOENVSUBST;
        shell_execute_info.nShow = static_cast<int>(entry_->ShowWindowOption());
        shell_execute_info.lpFile = execute_info_.command_line.command.c_str();

        auto arguments = zaf::JoinAsWideString(execute_info_.command_line.arguments);
        shell_execute_info.lpParameters = arguments.c_str();

        if (!execute_info_.working_directory.empty()) {
            shell_execute_info.lpDirectory = execute_info_.working_directory.c_str();
        }

        ShellExecuteEx(&shell_execute_info);
    }

private:
    std::shared_ptr<Entry> entry_;
    ExecuteInfo execute_info_;
};


context::ActivePath ModifyActivePath(
    const context::ActivePath& active_path,
    const std::wstring& modifier) {

    auto option = active_path::ParseActivePathOption(modifier.substr(1));
    return active_path::ModifyActivePathByOption(active_path, option);
}

}


UserDefinedCommand::UserDefinedCommand(
    std::shared_ptr<Entry> entry, 
    std::vector<utility::CommandLinePiece> input_arguments)
    :
    entry_(std::move(entry)),
    input_arguments_(std::move(input_arguments)) {

}


std::wstring UserDefinedCommand::GetKeyword() {
    return entry_->Command();
}


help::HelpContent UserDefinedCommand::GetHelpContent() {

    auto content_id = entry_->BundleMeta()->BundleID() + L'|' + entry_->Keyword();

    auto element = MakeRoot({
        MakeHeader(HeaderDepth::_2, {
            MakeInlineCode(entry_->Keyword()),
        }),
        help::BuildDescriptionParagraph(entry_->Description())
    });

    return { content_id, element };
}


bool UserDefinedCommand::Interpret(
    const utility::CommandLine& command_line, 
    const context::DesktopContext& desktop_context,
    bool is_reusing) {

    //Not allow to reuse.
    if (is_reusing) {
        return false;
    }

    desktop_context_ = desktop_context;
    return true;
}


std::shared_ptr<CommandPreviewControl> UserDefinedCommand::GetPreviewControl() {

    auto control = zaf::Create<UserDefinedCommandPreviewControl>();
    control->SetCommandEntry(entry_);
    control->SetExecutInfo(ParseCommand());
    return control;
}


std::shared_ptr<CommandExecutor> UserDefinedCommand::GetExecutor() {

    auto execute_info = ParseCommand();
    if (execute_info.command_line.command.empty()) {
        return nullptr;
    }

    return std::make_shared<UserDefinedCommandExecutor>(entry_, std::move(execute_info));
}


ExecuteInfo UserDefinedCommand::ParseCommand() const {

    context::ActivePath active_path;
    std::vector<std::wstring> plain_arguments;
    ParseInputArguments(active_path, plain_arguments);

    VariableFormatter variable_formatter{ entry_->BundleMeta(), active_path };

    ExecuteInfo result;
    result.command_line= user_defined::ParseEntryCommand(
        entry_->Command(),
        variable_formatter,
        plain_arguments);

    result.working_directory = variable_formatter.Format(entry_->WorkingDirectory(), {});
    return result;
}


void UserDefinedCommand::ParseInputArguments(
    context::ActivePath& modified_active_path,
    std::vector<std::wstring>& plain_arguments) const {

    modified_active_path = desktop_context_.active_path;

    for (auto index : zaf::Range(0, input_arguments_.size())) {

        const auto& each_argument = input_arguments_[index];
        if (each_argument.Type() == utility::CommandLinePieceType::NormalText) {

            //The first normal text argument starts with @ is the active path modifier.
            if (index == 0 && each_argument.Content()[0] == L'@') {
                modified_active_path = ModifyActivePath(
                    modified_active_path,
                    each_argument.Content());
            }
            else {
                plain_arguments.push_back(each_argument.Content());
            }
        }
        else if (each_argument.Type() == utility::CommandLinePieceType::TextBlock) {
            plain_arguments.push_back(each_argument.Content());
        }
        else if (each_argument.Type() == utility::CommandLinePieceType::ActivePath) {
            modified_active_path = context::ActivePath{ 
                each_argument.Content(), 
                desktop_context_.active_path.GetWorkspacePath()
            };
        }
    }
}

}