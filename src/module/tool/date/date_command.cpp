#include "module/tool/date/date_command.h"
#include <zaf/base/string/to_numeric.h>
#include <zaf/creation.h>
#include "module/common/command_error_control.h"
#include "utility/clipboard.h"

namespace ra::module::tool::date { 
namespace {

bool ParseSwitch(const std::wstring& argument, DateCommandParseResult& result) {

	if (argument[0] != L'/') {
		return false;
	}

	if (argument.size() <= 1) {
		return true;
	}

	if (argument[1] == L'e') {
		result.output_raw_value = true;
		return true;
	}

	return true;
}


bool ParseSingleArgument(const std::wstring& argument, DateCommandParseResult& result) {

	if (ParseSwitch(argument, result)) {
		return true;
	}

	std::time_t value{};
	if (zaf::TryToNumeric(argument, value)) {
		result.value = value;
		return true;
	}

	return false;
}

}

CommandBrief DateCommand::Brief() {
	return CommandBrief{
		L"date",
		L"Convert between date string and UNIX timestamp"
	};
}


std::optional<DateCommandParseResult> DateCommand::Parse(const utility::CommandLine& command_line) {

	DateCommandParseResult result;
	for (const auto& each_argument : command_line.Arguments()) {

		if (!ParseSingleArgument(each_argument, result)) {
			return std::nullopt;
		}
	}

	return result;
}


DateCommand::DateCommand(const utility::CommandLine& command_line) : 
	parse_result_(Parse(command_line)) {

}


CommandBrief DateCommand::GetBrief() {
	return Brief();
}


help::content::Content DateCommand::GetHelpContent() {

	help::content::Content result;
	result.AddTitleLine(L"Date command");
	result.AddBodyLine(L"Convert between human readable date time string and UNIX timestamp.");

	result.AddTitleLine(L"Usage");
	result.AddBodyLine(L"date [timestamp] [switches]");
	result.AddBodyLine(L"If `timestamp` is omitted, current date time would be used.");

	result.AddTitleLine(L"Switches");
	result.AddTwoPartsLine(L"/e", L"Show the result as UNIX timestamp.");

	return result;
}


std::shared_ptr<CommandPreviewControl> DateCommand::GetPreviewControl() {

	if (!parse_result_) {
		return CommandErrorControl::InvalidArgument();
	}

	if (!preview_control_) {
		preview_control_ = zaf::Create<DatePreviewControl>(*parse_result_);
	}

	return preview_control_;
}


void DateCommand::Execute() {

	if (!preview_control_) {
		return;
	}

	utility::SetStringToClipboard(preview_control_->GetText());
}

}