#include "module/date/date_preview_control.h"
#include <zaf/reflection/reflection_type_definition.h>
#include <zaf/rx/scheduler.h>
#include <zaf/rx/timer.h>

using namespace std::literals;

namespace ra::module::date {

ZAF_DEFINE_REFLECTION_TYPE(DatePreviewControl)
ZAF_DEFINE_RESOURCE_URI(L"res:///module/date/date_preview_control.xaml")
ZAF_DEFINE_END


DatePreviewControl::DatePreviewControl() {

}


DatePreviewControl::DatePreviewControl(const DateCommandParseResult& result) : 
	parse_result_(result) {

}


void DatePreviewControl::AfterParsing() {

	__super::AfterParsing();

	InitializeTextBox();

	if (parse_result_.value) {
		time_value_ = *parse_result_.value;
	}
	else {
		time_value_ = std::time(nullptr);
	}

	UpdateTextBox();

	//Start a timer to refresh current date time.
	StartTimerIfNeeded();
}


void DatePreviewControl::InitializeTextBox() {

	textBox->SetIsReadOnly(true);

	Subscriptions() += textBox->FocusChangeEvent().Subscribe(
		[this](const zaf::ControlFocusChangeInfo&) {
	
		if (!textBox->IsFocused()) {
			textBox->SetSelectionRange(zaf::TextRange{});
		}
	});

	Subscriptions() += textBox->DoubleClickEvent().Subscribe(
		[this](const zaf::ControlDoubleClickInfo&) {
	
		textBox->SetSelectionRange(zaf::TextRange{ 0, textBox->GetTextLength() });
	});

	Subscriptions() += textBox->SelectionChangeEvent().Subscribe(
		[this](const zaf::TextBoxSelectionChangeInfo&) {
	
		auto selection_range = textBox->GetSelectionRange();
		if (selection_range.length <= 0) {
			StartTimerIfNeeded();
		}
		else {
			timer_subscription_.reset();
		}
	});
}


void DatePreviewControl::StartTimerIfNeeded() {

	if (parse_result_.value) {
		return;
	}

	if (timer_subscription_.has_value()) {
		return;
	}

	timer_subscription_ = zaf::rx::Interval(1s, zaf::Scheduler::Main()).Subscribe([this](int) {

		time_value_ = std::time(nullptr);
		UpdateTextBox();
	});
}


void DatePreviewControl::UpdateTextBox() {

	auto selection_range = textBox->GetSelectionRange();

	auto time_text = GenerateTimeText();
	textBox->SetText(time_text);
	textBox->SetSelectionRange(selection_range);
}


std::wstring DatePreviewControl::GenerateTimeText() const {

	if (parse_result_.output_raw_value) {
		return std::to_wstring(time_value_);
	}

	std::tm tm{};
	auto error = localtime_s(&tm, &time_value_);
	if (error) {
		return L"Invalid value";
	}

	std::wostringstream stream;
	stream.imbue(std::locale(""));
	stream << std::put_time(&tm, L"%x %A %X %z");
	return stream.str();
}


std::wstring DatePreviewControl::GetText() const {
	return textBox->GetText();
}

}