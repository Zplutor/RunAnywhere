#pragma once

#include <zaf/base/none.h>
#include <zaf/control/rich_edit/embedded_object.h>
#include <zaf/rx/subject.h>
#include <zaf/rx/subscription_host.h>
#include <zaf/window/window.h>
#include "main/text_block_data.h"
#include "main/text_block_window.h"

namespace ra {

class TextBlockObject : public zaf::rich_edit::EmbeddedObject, zaf::SubscriptionHost {
public:
    explicit TextBlockObject(std::wstring text);

    const std::shared_ptr<TextBlockData>& Data() const {
        return data_;
    }

    const std::wstring& Text() const {
        return data_->Text();
    }

    void OpenWindow();

    zaf::Observable<zaf::None> TextChangedEvent() {
        return text_changed_event_.GetObservable();
    }

    GUID ClassID() const override;

    void Paint(
        zaf::Canvas& canvas,
        const zaf::Rect& dirty_rect,
        const zaf::rich_edit::PaintContext& context) override;

    void OnMouseCursorChanging(const zaf::rich_edit::MouseCursorChangingContext& context) override;
    bool OnDoubleClick(const zaf::rich_edit::DoubleClickContext& context) override;

private:
    void PaintText(zaf::Canvas& canvas, const zaf::Rect& text_rect);
    bool InnerOpenWindow(const zaf::Point& object_position_in_screen);
    void OnTextChanged(const std::shared_ptr<TextBlockWindow>& window);
    void OnWindowDestroyed();

private:
    std::shared_ptr<TextBlockData> data_;
    zaf::Subject<zaf::None> text_changed_event_;
};

}