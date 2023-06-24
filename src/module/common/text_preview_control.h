#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/rich_edit.h>
#include <zaf/control/scrollable_control.h>
#include "module/command_preview_control.h"

namespace ra::mod {

class TextPreviewControl : public CommandPreviewControl {
public:
    enum class DisplayMode {
        Normal,
        Base64,
    };

public:
    ZAF_DECLARE_TYPE;

public:
    void SetDisplayMode(DisplayMode mode);
    void SetText(std::wstring text);

protected:
    zaf::Frame GetExpectedMargin() override;
    void Layout(const zaf::Rect&) override;

private:
    void ShowEmptyText();
    void ShowText();

    void CalculateAndAdjustControls();
    bool TryToAdjustForSingleLineText();
    bool DeterminateIfAllTextCanFitInSingleLine(bool& has_set_text);
    float GetTextLayoutWidth() const;
    void BreakTextForBase64Mode();
    void AdjustForMultiLineText(bool has_set_text);
    static float CalculateRequriedHeightForMultiLineEdit(
        const zaf::Size& text_preferrence_size,
        std::size_t line_count,
        const zaf::Size& text_bounds);

private:
    ZAF_BIND_CONTROL(zaf::Label, emptyLabel);
    ZAF_BIND_CONTROL(zaf::ScrollableControl, scrollControl);
    ZAF_BIND_CONTROL(zaf::RichEdit, richEdit);

    DisplayMode display_mode_{ DisplayMode::Normal };
    std::wstring text_;
    bool has_line_break_{};
};

}