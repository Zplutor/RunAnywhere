#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include "module/command_preview_control.h"
#include "module/user_defined/execute_info.h"

namespace ra::mod::user_defined {

class UserDefinedCommandPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

    void SetExecutInfo(ExecuteInfo execute_info);

protected:
    void AfterParse() override;
    void OnStyleChanged() override;

private:
    void BuildArgumentLabels(const std::vector<std::wstring>& arguments);
    void ResetFixedHeight();

private:
    ZAF_BIND_CONTROL(zaf::Label, commandLabel);
    ZAF_BIND_CONTROL(zaf::Control, argumentContainer);
    ZAF_BIND_CONTROL(zaf::Label, workingDirectoryLabel);

    ExecuteInfo execute_info_;
};

}