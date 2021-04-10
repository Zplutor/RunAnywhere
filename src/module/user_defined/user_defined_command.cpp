#include "module/user_defined/user_defined_command.h"
#include <Windows.h>

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

}

UserDefinedCommand::UserDefinedCommand(
    const UserDefinedEntry& entry, 
    const std::vector<std::wstring>& input_arguments)
    :
    entry_(entry),
    input_arguments_(input_arguments) {

}


std::wstring UserDefinedCommand::GetPreviewText() {

    auto parse_result = ParseCommandLine();
    return parse_result.command + L' ' + JoinArguments(parse_result.arguments);
}


void UserDefinedCommand::Execute() {

    auto parse_result = ParseCommandLine();

    ShellExecute(
        nullptr, 
        nullptr, 
        parse_result.command.c_str(),
        JoinArguments(parse_result.arguments).c_str(),
        nullptr, 
        SW_SHOWNORMAL);
}


ParseResult UserDefinedCommand::ParseCommandLine() {

    return ::ParseCommandLine(
        entry_.command_line,
        GetDesktopContext().current_focused_path, 
        input_arguments_);
}

