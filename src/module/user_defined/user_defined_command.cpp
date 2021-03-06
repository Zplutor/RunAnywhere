#include "module/user_defined/user_defined_command.h"
#include <Windows.h>
#include <zaf/base/container/utility/range.h>
#include <zaf/creation.h>
#include "environment_variable_manager.h"
#include "module/active_path/active_path_modifying.h"
#include "module/active_path/active_path_option_parsing.h"
#include "module/user_defined/preview/user_defined_command_preview_control.h"

namespace ra::module::user_defined {
namespace {

std::wstring JoinArguments(const std::vector<std::wstring>& arguments) {

    std::wstring result;

    for (std::size_t index = 0; index < arguments.size(); ++index) {

        if (index != 0) {
            result += L' ';
        }

        result += arguments[index];
    }

    return result;
}


context::ActivePath ModifyActivePath(
    const context::ActivePath& active_path,
    const std::wstring& modifier) {

    auto option = active_path::ParseActivePathOption(modifier.substr(1));
    return active_path::ModifyActivePathByOption(active_path, option);
}

}


UserDefinedCommand::UserDefinedCommand(
    const UserDefinedEntry& entry, 
    const std::vector<std::wstring>& input_arguments)
    :
    entry_(entry),
    input_arguments_(input_arguments) {

}


help::content::Content UserDefinedCommand::GetHelpContent() {

    help::content::Content result;
    result.AddTitleLine(entry_.keyword + L" command");
    result.AddBodyLine(L"User-defined command.");
    return result;
}


std::shared_ptr<CommandPreviewControl> UserDefinedCommand::GetPreviewControl() {

    auto control = zaf::Create<UserDefinedCommandPreviewControl>();
    control->SetParseResult(ParseCommandLine());
    return control;
}


void UserDefinedCommand::Execute() {

    auto parse_result = ParseCommandLine();

    //Update current process' environment variables in order to inherit them in child process.
    EnvironmentVariableManager::Instance().Update();

    ShellExecute(
        nullptr,
        nullptr,
        parse_result.command.c_str(),
        JoinArguments(parse_result.arguments).c_str(),
        nullptr,
        SW_SHOWNORMAL);
}


ParseResult UserDefinedCommand::ParseCommandLine() {

    context::ActivePath modified_active_path;
    std::vector<std::wstring> plain_arguments;
    ParseArguments(modified_active_path, plain_arguments);

    return user_defined::ParseCommandLine(
        entry_.command_line,
        modified_active_path, 
        plain_arguments);
}


void UserDefinedCommand::ParseArguments(
    context::ActivePath& modified_active_path,
    std::vector<std::wstring>& plain_arguments) {

    modified_active_path = GetDesktopContext().active_path;

    if (input_arguments_.empty()) {
        return;
    }

    const auto& first_argument = input_arguments_[0];
    if (first_argument[0] == L'@') {
        modified_active_path = ModifyActivePath(modified_active_path, first_argument);
    }
    else {
        plain_arguments.push_back(first_argument);
    }

    for (std::size_t index = 1; index < input_arguments_.size(); ++index) {
        plain_arguments.push_back(input_arguments_[index]);
    }
}

}