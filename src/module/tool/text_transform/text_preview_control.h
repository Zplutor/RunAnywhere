#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/rich_edit.h>
#include <zaf/control/scrollable_control.h>
#include "module/command_preview_control.h"

namespace ra::module::tool::text_transform {

class TextPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

public:
    void SetText(const std::wstring& text);

protected:
    zaf::Frame GetExpectedMargin() override;
    void Layout(const zaf::Rect&) override;

private:
    void CalculateAndAdjustControls();
    void AdjustForSingleLineEdit();
    void AdjustForMultiLineEdit();
    static float CalculateRequriedHeightForMultiLineEdit(
        const zaf::Size& edit_size,
        std::size_t line_count,
        const zaf::Size& bounds,
        bool& need_horizontal_scroll, 
        bool& need_vertical_scroll);

private:
    ZAF_BIND_CONTROL(zaf::ScrollableControl, scrollControl);
    ZAF_BIND_CONTROL(zaf::RichEdit, richEdit);
};

}