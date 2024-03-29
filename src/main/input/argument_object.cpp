#include "main/input/argument_object.h"
#include <tom.h>
#include <zaf/base/as.h>
#include <zaf/creation.h>
#include <zaf/control/rich_edit.h>
#include <zaf/graphic/canvas.h>
#include <zaf/graphic/graphic_factory.h>
#include <zaf/graphic/text/text_format_properties.h>
#include "utility/text_utility.h"

namespace ra::main::input {

ArgumentObject::ArgumentObject(std::shared_ptr<ArgumentData> data) : data_(std::move(data)) {

    SetStyle(CommandDisplayStyle::Normal);
}


void ArgumentObject::SetStyle(CommandDisplayStyle style) {

    style_ = style;

    this->SetSize(
        style_ == CommandDisplayStyle::Preserved ? zaf::Size{ 48, 18 } : zaf::Size{ 60, 28 });
}


void ArgumentObject::Paint(
    zaf::Canvas& canvas,
    const zaf::Rect& dirty_rect,
    const zaf::rich_edit::PaintContext& context) {

    zaf::RoundedRect rounded_rect;
    rounded_rect.rect.size = this->Size();
    rounded_rect.rect.Deflate(zaf::Frame{ 2, 1, 2, 1 });
    rounded_rect.x_radius = 4;
    rounded_rect.y_radius = 4;

    auto background_color = GetBackgroundColor(context);
    canvas.DrawRoundedRectangle(rounded_rect, background_color);

    PaintText(canvas, rounded_rect.rect);
}


void ArgumentObject::PaintText(zaf::Canvas& canvas, const zaf::Rect& text_rect) {

    zaf::TextFormatProperties text_format_properties;
    text_format_properties.font_size = style_ == CommandDisplayStyle::Preserved ? 12.f : 16.f;
    auto text_format = zaf::GraphicFactory::Instance().CreateTextFormat(text_format_properties);

    zaf::TextTrimming text_trimming;
    text_trimming.SetGranularity(zaf::TextTrimmingGranularity::Character);
    text_trimming.SetSign(zaf::GraphicFactory::Instance().CreateEllipsisTrimmingSign(text_format));
    text_format.SetTextTrimming(text_trimming);

    text_format.SetWordWrapping(zaf::WordWrapping::NoWrap);
    text_format.SetTextAlignment(zaf::TextAlignment::Center);
    text_format.SetParagraphAlignment(zaf::ParagraphAlignment::Center);

    zaf::Rect paint_rect = text_rect;
    paint_rect.Deflate(zaf::Frame{ 4, 0, 3, 0 });

    canvas.DrawTextFormat(
        utility::ReplaceWhitespacesToVisibleChars(Text()),
        text_format,
        paint_rect,
        zaf::Color::Black());
}


void ArgumentObject::OnMouseCursorChanging(
    const zaf::rich_edit::MouseCursorChangingContext& context) {

    SetCursor(LoadCursor(nullptr, IDC_ARROW));
    context.EventInfo().MarkAsHandled();
}


bool ArgumentObject::OnDoubleClick(const zaf::rich_edit::DoubleClickContext& context) {
    return InnerOpenWindow(context.ObjectPositionInScreen());
}


void ArgumentObject::OpenWindow() {

    try {

        auto position = this->GetPositionInScreen();
        if (position) {
            InnerOpenWindow(*position);
        }
    }
    catch (const zaf::Error&) {

    }
}


bool ArgumentObject::InnerOpenWindow(const zaf::Point& object_position_in_screen) {

    auto host = Host();
    if (!host) {
        return false;
    }

    auto host_window = host->Window();
    if (!host_window) {
        return false;
    }

    auto window = CreateArgumentObjectWindow();
    window->SetOwner(host_window);
    window->SetInitialRectStyle(zaf::InitialRectStyle::Custom);

    window->SetObjectPositionInScreen(object_position_in_screen);
    window->SetIsReadOnly(style_ != CommandDisplayStyle::Normal);
    window->SetText(Text());

    Subscriptions() += window->TextChangedEvent().Subscribe(
        std::bind(&ArgumentObject::OnTextChanged, this, std::placeholders::_1));

    Subscriptions() += window->DestroyedEvent().Subscribe(
        std::bind(&ArgumentObject::OnWindowDestroyed, this));

    window->Show();
    return true;
}


void ArgumentObject::OnTextChanged(const std::shared_ptr<ArgumentObjectWindow>& window) {

    auto new_text = window->GetText();

    //Repaint only if several head chars are different. 
    constexpr std::size_t CompareCount = 10;
    bool need_repaint = new_text.compare(0, CompareCount, Text(), 0, CompareCount) != 0;

    data_ = CreateData(std::move(new_text));

    if (need_repaint) {
        this->NeedRepaint();
    }

    text_changed_event_.GetObserver().OnNext({});
}


void ArgumentObject::OnWindowDestroyed() {

    //Remove current object from rich edit if text is empty.
    if (!Text().empty()) {
        return;
    }

    try {

        auto char_index = this->GetCharIndex();
        if (!char_index) {
            return;
        }

        auto host = Host();
        if (!host) {
            return;
        }

        auto text_document = host->GetOLEInterface().Inner().Query<ITextDocument>();
        if (!text_document) {
            return;
        }

        zaf::COMPtr<ITextRange> text_range;
        HRESULT hresult = text_document->Range(
            static_cast<long>(*char_index),
            static_cast<long>(*char_index) + 1,
            text_range.Reset());

        if (FAILED(hresult)) {
            return;
        }

        hresult = text_range->Delete(tomCharacter, 0, nullptr);
        if (FAILED(hresult)) {
            return;
        }
    }
    catch (const zaf::Error&) {

    }
}

}