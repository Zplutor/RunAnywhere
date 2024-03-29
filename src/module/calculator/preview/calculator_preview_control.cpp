#include "module/calculator/preview/calculator_preview_control.h"
#include <zaf/graphic/font/font.h>
#include <zaf/object/type_definition.h>
#include "module/calculator/result_text_builder.h"
#include "utility/numeric_text_formatting.h"

namespace ra::mod::calculator {
namespace {

constexpr float NormalStyleFixedHeight = 90;
constexpr float HistoricalStyleFixedHeight = 28;

constexpr float NormalStyleDefaultFontSize = 32;
constexpr float HistoricalStyleDefaultFontSize = 16;

}

ZAF_DEFINE_TYPE(CalculatorPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/calculator/preview/calculator_preview_control.xaml");
ZAF_DEFINE_TYPE_END

void CalculatorPreviewControl::OnStyleChanged() {

	if (Style() == CommandDisplayStyle::Preserved) {
		this->SetFixedHeight(HistoricalStyleFixedHeight);
	}
	else {
		this->SetFixedHeight(NormalStyleFixedHeight);
	}

	NeedRelayout();
}


void CalculatorPreviewControl::Layout(const zaf::Rect& previous_rect) {

	ResizetLabelToSuitableSize();
	RePositionLabel();
}


void CalculatorPreviewControl::ResizetLabelToSuitableSize() {

	auto content_size = ContentSize();
	if (content_size.width == 0) {
		return;
	}

	if (resultLabel->Text().length() == 0) {
		return;
	}

	const float default_font_size =
		Style() == CommandDisplayStyle::Preserved ?
		HistoricalStyleDefaultFontSize :
		NormalStyleDefaultFontSize;

	for (float font_size = default_font_size; font_size > 0; font_size -= 0.5) {

		resultLabel->SetFontSize(font_size);

		auto result_label_size = resultLabel->CalculatePreferredSize();
		auto result_label_margin = resultLabel->Margin();

		auto total_width =
			result_label_size.width +
			result_label_margin.left +
			result_label_margin.right;

		if (total_width <= content_size.width) {

			resultLabel->SetSize(result_label_size);
			break;
		}
	}
}


void CalculatorPreviewControl::RePositionLabel() {

	auto content_size = this->ContentSize();
	const auto& result_label_size = resultLabel->Size();

	zaf::Point result_label_position;
	result_label_position.y = (content_size.height - result_label_size.height) / 2;

	if (Style() == CommandDisplayStyle::Preserved) {
		result_label_position.x = 0;
	}
	else {
		result_label_position.x = (content_size.width - result_label_size.width) / 2;
	}

	resultLabel->SetPosition(result_label_position);
}


void CalculatorPreviewControl::SetResult(
	const calculator::EvaluateResult& evaluate_result,
	const calculator::Modifier& modifier) {

	evaluate_result_ = evaluate_result;
	modifier_ = modifier;

	UpdateResult();
}


void CalculatorPreviewControl::UpdateResult() {

	SetTextToLabel();
	ShowHighlightBit();
	NeedRelayout();
}


void CalculatorPreviewControl::SetTextToLabel() {

	ResultTextBuilder text_builder(evaluate_result_, modifier_);
	auto result_text = text_builder.Build();

	auto label_text = result_text.content;

	utility::InsertSeparatorToNumericText(label_text, modifier_.base, L' ');

	if (!result_text.prefix.empty()) {

		label_text = result_text.prefix + L' ' + label_text;
		prefix_length_ = result_text.prefix.length() + 1;
	}

	resultLabel->SetText(label_text);
}


void CalculatorPreviewControl::ShowHighlightBit() {

	auto highlight_position = GetHighlightBitPositionInResultLabel();
	if (!highlight_position) {
		return;
	}

	resultLabel->SetTextColorAtRange(
		zaf::Color::Red(),
		zaf::Range{ *highlight_position, 1 });
}


std::optional<std::size_t> CalculatorPreviewControl::GetHighlightBitPositionInResultLabel() {

	if (!modifier_.highlight_bit || modifier_.base != 2) {
		return std::nullopt;
	}

	auto text = resultLabel->Text();

	int current_bit = 0;
	for (std::size_t position = text.length() - 1;
		position >= prefix_length_ && position < text.length();
		--position) {

		if (text[position] == L' ') {
			continue;
		}

		if (current_bit == *modifier_.highlight_bit) {
			return position;
		}

		++current_bit;
	}

	return std::nullopt;
}

}