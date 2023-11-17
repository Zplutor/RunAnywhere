#include "option/option_text_box.h"
#include <zaf/object/type_definition.h>

namespace ra::option {

ZAF_DEFINE_TYPE(OptionTextBox)
ZAF_DEFINE_TYPE_END;

void OptionTextBox::AfterParse() {

    __super::AfterParse();

    Subscriptions() += TextChangedEvent().Subscribe(std::bind([this]() {
        value_changed_ = true;
    }));

    Subscriptions() += FocusLostEvent().Subscribe(std::bind([this]() {
        CommitValueChanged();
    }));

    Subscriptions() += KeyDownEvent().Subscribe([this](const zaf::KeyDownInfo& event_info) {
        if (event_info.Message().VirtualKey() == VK_RETURN) {
            CommitValueChanged();
            event_info.MarkAsHandled();
        }
    });
}


void OptionTextBox::CommitValueChanged() {
    if (value_changed_) {
        value_changed_event_.AsObserver().OnNext(Text());
        value_changed_ = false;
    }
}

}