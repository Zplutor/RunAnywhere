#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/linear_box.h>
#include <zaf/control/text_box.h>
#include "module/chat_gpt/dialog.h"
#include "module/chat_gpt/dialog/answer_view.h"

namespace ra::mod::chat_gpt {

class RoundView : public zaf::VerticalBox {
public:
    ZAF_DECLARE_TYPE;

    explicit RoundView(std::shared_ptr<Dialog> dialog);

    void Start(std::wstring question);

protected:
    void AfterParse() override;

private:
    ZAF_BIND_CONTROL(zaf::Control, questionView);
    ZAF_BIND_CONTROL(zaf::TextBox, questionContent);
    ZAF_BIND_CONTROL(AnswerView, answerView);

    std::shared_ptr<Dialog> dialog_;
};

}