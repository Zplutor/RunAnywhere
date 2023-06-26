#include "module/common/binary_content/binary_content_control.h"
#include <zaf/control/scroll_bar.h>
#include <zaf/control/scroll_bar_thumb.h>
#include <zaf/graphic/canvas.h>
#include <zaf/object/type_definition.h>

namespace ra::mod {

ZAF_DEFINE_TYPE(BinaryContentControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/common/binary_content/binary_content_control.xaml")
ZAF_DEFINE_TYPE_END;

void BinaryContentControl::AfterParse() {

    __super::AfterParse();

    auto scroll_bar = scrollControl->VerticalScrollBar();
    scroll_bar->SetSmallChange(static_cast<int>(LineHeight));

    //If there is too much data, cached painting will fail, so we have to disable it.
    scrollControl->ScrollContent()->SetIsCachedPaintingEnabled(false);
}


void BinaryContentControl::Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) {

    __super::Paint(canvas, dirty_rect);

    canvas.DrawRectangle(
        zaf::Rect{ 0, 0, LineHeaderWidth, this->ContentSize().height },
        HeaderBackgroundColor());
}


void BinaryContentControl::SetBinary(std::vector<std::byte> binary) {

    body->SetBinary(std::move(binary));

    auto body_size = body->CalculatePreferredSize();

    float scroll_height = std::max(body_size.height, 90.f - LineHeight);//LineHeight is the header.
    scroll_height = std::min(scroll_height, LineHeight * 8.f);
    scrollControl->SetFixedHeight(scroll_height);
}

}