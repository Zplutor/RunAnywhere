#include "module/active_path/active_path_module.h"
#include "module/active_path/active_path_command.h"
#include "module/active_path/active_path_option_parsing.h"

namespace ra::module::active_path {
namespace {

constexpr wchar_t PrefixChar = L'@';

}


std::vector<CommandBrief> ActivePathModule::QuerySuggestedCommands(
	const std::wstring& command_text) {

	if (command_text.empty() || command_text.front() == PrefixChar) {

		return { 
			CommandBrief{ std::wstring(1, PrefixChar), L"Show active path" } 
		};
	}

	return {};
}


std::shared_ptr<Command> ActivePathModule::Interpret(const utility::CommandLine& command_line) {

	const auto& command_line_text = command_line.Text();
	if (command_line_text.empty()) {
		return nullptr;
	}

	if (command_line_text.front() != PrefixChar) {
		return nullptr;
	}

	auto option = ParseActivePathOption(command_line_text.substr(1));
	return std::make_shared<ActivePathCommand>(option);
}

}