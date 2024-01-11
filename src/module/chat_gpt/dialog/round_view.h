#pragma once

#include <zaf/base/none.h>
#include <zaf/control/button.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/linear_box.h>
#include <zaf/control/text_box.h>
#include "module/chat_gpt/dialog/answer_view.h"
#include "module/chat_gpt/dialog/round.h"

namespace ra::mod::chat_gpt {

class RoundView : public zaf::VerticalBox {
public:
    ZAF_DECLARE_TYPE;

    explicit RoundView(std::shared_ptr<Round> round);

    std::shared_ptr<chat_gpt::AnswerView> AnswerView() const {
        return answerView;
    }

    const std::shared_ptr<Round>& Round() const {
        return round_;
    }

protected:
    void AfterParse() override;
    void OnMouseEnter(const zaf::MouseEnterInfo& event_info) override;
    void OnMouseLeave(const zaf::MouseLeaveInfo& event_info) override;

private:
    enum class RoundState {
        Requesting,
        Error,
        Finished,
    };

private:
    zaf::Observable<std::wstring> ObserveAnswer();
    void ChangeState(RoundState state);
    void UpdateToolbarState();

private:
    ZAF_BIND_CONTROL(zaf::Control, questionView);
    ZAF_BIND_CONTROL(zaf::TextBox, questionContent);
    ZAF_BIND_CONTROL(chat_gpt::AnswerView, answerView);
    ZAF_BIND_CONTROL(zaf::Control, toolbar);
    ZAF_BIND_CONTROL(zaf::Button, copyButton);
    ZAF_BIND_CONTROL(zaf::Button, removeButton);

    std::shared_ptr<chat_gpt::Round> round_;
    RoundState state_{ RoundState::Requesting };
};

}