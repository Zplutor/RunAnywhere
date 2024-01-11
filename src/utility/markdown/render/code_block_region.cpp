#include "utility/markdown/render/code_block_region.h"
#include <zaf/base/log.h>
#include <zaf/base/error/check.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/control/scroll_bar.h>
#include <zaf/creation.h>
#include <zaf/object/type_definition.h>
#include "utility/clipboard.h"
#include "utility/markdown/element/code_block_element.h"

namespace ra::utility::markdown::render {

ZAF_DEFINE_TYPE(CodeBlockRegion)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///utility/markdown/render/code_block_region.xaml")
ZAF_DEFINE_TYPE_END;

std::shared_ptr<CodeBlockRegion> CodeBlockRegion::Create(
    const element::Element& element, 
    const StyleConfig& style_config) {

    ZAF_EXPECT(element.Type() == element::ElementType::CodeBlock);
    const auto& code_block_element = dynamic_cast<const element::CodeBlockElement&>(element);

    auto result = zaf::Init(new CodeBlockRegion());
    if (!code_block_element.Children().empty()) {
        TextStyle text_style;
        text_style.font = style_config.basic_config.font;
        text_style.font.family_name = style_config.code_block_config.code_config.font_family_name;
        text_style.text_color = style_config.code_block_config.code_config.text_color;
        result->SetStyledText(code_block_element.Children().front()->Text(), text_style);
    }
    result->SetShowHeader(style_config.code_block_config.show_header);
    result->SetLanguage(code_block_element.Language());
    result->SetTextBackgroundColor(style_config.code_block_config.code_config.background_color);
    return result;
}


void CodeBlockRegion::AfterParse() {

    __super::AfterParse();

    Subscriptions() += copyButton->ClickEvent().Subscribe(std::bind([this]() {
         utility::SetStringToClipboard(textBox->Text());
    }));
}


zaf::Size CodeBlockRegion::CalculatePreferredContentSize(const zaf::Size& bound_size) const {

    auto result = textBox->CalculatePreferredSize(bound_size);
    if (result.width > bound_size.width) {
        result.height += scrollControl->HorizontalScrollBar()->Height();
    }

    result.height += scrollControl->Margin().Height();

    if (header->IsVisible()) {
        result.height += header->Height() + header->Margin().Height();
    }
    return result;
}


void CodeBlockRegion::OnMouseEnter(const zaf::MouseEnterInfo& event_info) {

    __super::OnMouseEnter(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    copyButton->SetIsVisible(true);
}


void CodeBlockRegion::OnMouseLeave(const zaf::MouseLeaveInfo& event_info) {

    __super::OnMouseLeave(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    if (!this->ContainMouse()) {
        copyButton->SetIsVisible(false);
    }
}


void CodeBlockRegion::SetStyledText(const std::wstring& text, const TextStyle& text_style) {
    textBox->SetText(text);
    textBox->SetFont(text_style.font);
    textBox->SetTextColor(text_style.text_color);
}


void CodeBlockRegion::SetShowHeader(bool show) {
    header->SetIsVisible(show);
}


void CodeBlockRegion::SetLanguage(const std::wstring& language) {

    if (!language.empty()) {
        languageLabel->SetText(language);
    }
    else {
        languageLabel->SetText(L"text");
    }
}


void CodeBlockRegion::SetTextBackgroundColor(const zaf::Color& color) {

    this->SetBackgroundColor(color);
    textBox->SetBackgroundColor(color);
}


bool CodeBlockRegion::IsPositionInsideTextBoundary(const zaf::Point& mouse_position) {

    auto position_in_text_box = this->TranslatePositionToChild(mouse_position, *scrollControl);

    if (textBox->RectInSelf().Contain(position_in_text_box)) {
        return textBox->IsPositionInsideText(position_in_text_box);
    }

    return false;
}


void CodeBlockRegion::BeginSelection(const zaf::Point& position) {

    if (!IsPositionInTextBox(position)) {
        return;
    }

    auto horizontal_scroll_bar = scrollControl->HorizontalScrollBar();
    begin_selection_x_offset_ = static_cast<float>(horizontal_scroll_bar->Value());
}


void CodeBlockRegion::ChangeSelection(const composite::PositionRange& position_range) {

    auto begin_position_in_text_box =
        this->TranslatePositionToChild(position_range.Begin(), *scrollControl);

    bool scroll_to_selection{ false };
    if (begin_selection_x_offset_) {

        auto horizontal_scroll_bar = scrollControl->HorizontalScrollBar();
        auto current_x_offset = static_cast<float>(horizontal_scroll_bar->Value());

        begin_position_in_text_box.x -= current_x_offset - *begin_selection_x_offset_ ;

        //If both begin and end position are in the rectangle of the text box, scroll to the 
        //selection.
        if (IsPositionInTextBox(position_range.End())) {
            scroll_to_selection = true;
        }
    }

    PositionRange position_range_in_text_box{ 
        begin_position_in_text_box,
        this->TranslatePositionToChild(position_range.End(), *scrollControl) 
    };
    textBox->SetSelectionByPositionRange(position_range_in_text_box, scroll_to_selection);
}


bool CodeBlockRegion::IsPositionInTextBox(const zaf::Point& position) const {

    auto position_in_text_box = this->TranslatePositionToChild(position, *scrollControl);
    return 
        (position_in_text_box.y >= 0) && 
        (position_in_text_box.y < textBox->Height());
}


void CodeBlockRegion::EndSelection() {
    begin_selection_x_offset_.reset();
}


void CodeBlockRegion::SelectWord(const zaf::Point& position) {

    if (!IsPositionInTextBox(position)) {
        return;
    }

    auto position_in_text_box = this->TranslatePositionToChild(position, *scrollControl);
    textBox->SelectWord(position_in_text_box);
}


void CodeBlockRegion::BuildSelectedText(composite::SelectedTextBuilder& builder) {
    textBox->BuildSelectedText(builder);
}


void CodeBlockRegion::ChangeFocus(bool is_focused) {
    textBox->ChangeFocus(is_focused);
}

}