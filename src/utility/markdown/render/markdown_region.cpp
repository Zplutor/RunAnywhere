#include "utility/markdown/render/markdown_region.h"
#include <zaf/base/error/check.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/creation.h>
#include "utility/clipboard.h"

namespace ra::utility::markdown::render {

std::shared_ptr<MarkdownRegion> MarkdownRegion::Create(
    const element::Element& element,
    const StyleConfig& style_config) {

    ZAF_EXPECT(element.Type() == element::ElementType::Root);

    auto body_region = BodyRegion::Create(element.Children(), style_config, 0);
    return zaf::Init(new MarkdownRegion(body_region));
}


MarkdownRegion::MarkdownRegion(std::shared_ptr<BodyRegion> body_region) :
    body_region_(std::move(body_region)) {

}


void MarkdownRegion::Initialize() {

    __super::Initialize();

    this->SetLayouter(zaf::Create<zaf::VerticalLayouter>());
    AddChild(body_region_);
}


void MarkdownRegion::SetCanSelect(bool can_select) {

    can_select_ = can_select;
    this->SetCanFocused(can_select_);
}


zaf::Size MarkdownRegion::CalculatePreferredContentSize(const zaf::Size& bound_size) const {
    return body_region_->CalculatePreferredSize(bound_size);
}


void MarkdownRegion::OnMouseDown(const zaf::MouseDownInfo& event_info) {

    __super::OnMouseDown(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    if (!can_select_) {
        return;
    }

    this->SetIsFocused(true);

    if (event_info.Message().MouseButton() != zaf::MouseButton::Left) {
        return;
    }

    begin_selection_position_ = this->TranslatePositionToChild(
        event_info.PositionAtSender(),
        *body_region_);

    body_region_->BeginSelection(*begin_selection_position_);
    body_region_->ChangeSelection(PositionRange{
        *begin_selection_position_, 
        *begin_selection_position_ 
    });

    CaptureMouse();
    event_info.MarkAsHandled();
}


void MarkdownRegion::OnMouseUp(const zaf::MouseUpInfo& event_info) {

    __super::OnMouseUp(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    if (!can_select_) {
        return;
    }

    if (begin_selection_position_) {
        begin_selection_position_.reset();

        body_region_->EndSelection();
        ReleaseMouse();
        event_info.MarkAsHandled();
    }
}


void MarkdownRegion::OnPreMouseMove(const zaf::PreMouseMoveInfo& event_info) {

    __super::OnPreMouseMove(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    if (!can_select_) {
        return;
    }

    if (!begin_selection_position_) {
        return;
    }

    auto current_position = this->TranslatePositionToChild(
        event_info.PositionAtSender(),
        *body_region_);

    body_region_->ChangeSelection(PositionRange{
        *begin_selection_position_,
        current_position
    });

    event_info.MarkAsHandled();
}


void MarkdownRegion::OnKeyDown(const zaf::KeyDownInfo& event_info) {

    __super::OnKeyDown(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    if (!can_select_) {
        return;
    }

    if ((GetKeyState(VK_CONTROL) >> 15) && event_info.Message().VirtualKey() == L'C') {

        SelectedTextBuilder text_builder;
        body_region_->BuildSelectedText(text_builder);

        if (!text_builder.Text().empty()) {
            SetStringToClipboard(text_builder.Text());
        }

        event_info.MarkAsHandled();
    }
}


void MarkdownRegion::OnFocusGained(const zaf::FocusGainedInfo& event_info) {

    __super::OnFocusGained(event_info);

    if (!can_select_) {
        return;
    }

    body_region_->ChangeFocus(true);
    event_info.MarkAsHandled();
}


void MarkdownRegion::OnFocusLost(const zaf::FocusLostInfo& event_info) {

    __super::OnFocusLost(event_info);

    if (!can_select_) {
        return;
    }

    body_region_->ChangeFocus(false);
    event_info.MarkAsHandled();
}

}