#include "module/meta/meta_module.h"
#include "module/meta/meta_command.h"
#include "module/meta/meta_command_info.h"
#include "module/meta/meta_command_prefix.h"

namespace ra::module::meta {
namespace {

std::optional<std::wstring> ExtractMetaCommand(const std::wstring& command_text) {

    if (command_text.empty()) {
        return std::nullopt;
    }

    if (command_text.front() != MetaCommandPrefix) {
        return std::nullopt;
    }

    return command_text.substr(1);
}

}


std::vector<CommandBrief> MetaModule::QuerySuggestedCommands(const std::wstring& command_text) {

    std::wstring command;
    if (!command_text.empty()) {

        auto meta_command = ExtractMetaCommand(command_text);
        if (!meta_command) {
            return {};
        }

        command = *meta_command;
    }

    std::vector<CommandBrief> result;
    
    for (const auto& each_info : GetMetaCommandInfos()) {

        if (each_info.command.find(command) != 0) {
            continue;
        }

        result.emplace_back(MetaCommandPrefix + each_info.command, each_info.description);
    }

    return result;
}


std::shared_ptr<Command> MetaModule::Interpret(const utility::CommandLine& command_line) {

    auto command = ExtractMetaCommand(command_line.Text());
    if (!command || command->empty()) {
        return nullptr;
    }

    auto command_info = GetMetaCommandInfo(*command);
    if (!command_info) {
        return nullptr;
    }

    return std::make_shared<MetaCommand>(*command_info);
}

}