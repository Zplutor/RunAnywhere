#pragma once

#include <zaf/window/window.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/text_box.h>
#include "context/desktop_context.h"
#include "module/command_preview_control.h"
#include "module/module.h"
#include "module/user_defined/user_defined_module.h"
#include "preview_view.h"

namespace ra {

class MainWindow : public zaf::Window {
public:
    ZAF_DECLARE_REFLECTION_TYPE;

    static MainWindow& Instance();

    void ShowOnTop();
    void ReloadUserDefinedCommands();

protected:
    void AfterParse() override;

    bool ReceiveMessage(const zaf::Message& message, LRESULT& result) override;
    std::optional<zaf::HitTestResult> HitTest(const zaf::HitTestMessage& message) override;
    void OnWindowShown() override;

private:
    void InitializeTextBox();
    void InitializeModules();

    void OnTextChanged(const zaf::TextualControlTextChangeInfo&);
    void InterpretCommand(const std::wstring& input);
    void ShowPreview();
    float ShowCommandPreview();
    std::shared_ptr<module::CommandPreviewControl> CreateDefaultPreviewControl(
        const std::wstring& preview_text);
    float ShowEmptyPreview();
    void ExecuteCommand();

private:
    ZAF_BIND_CONTROL(zaf::TextBox, inputTextBox);
    ZAF_BIND_CONTROL(PreviewView, previewView);

    float initial_height_{};
    std::shared_ptr<module::user_defined::UserDefinedModule> user_defined_module_;
    std::vector<std::shared_ptr<module::Module>> modules_;

    context::DesktopContext desktop_context_;
    std::shared_ptr<module::Command> current_command_;
};

}