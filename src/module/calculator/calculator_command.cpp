#include "module/calculator/calculator_command.h"
#include <zaf/creation.h>
#include "help/built_in_help_content_manager.h"
#include "module/calculator/preview/calculator_preview_control.h"
#include "module/calculator/result_text_builder.h"
#include "module/common/copy_executor.h"

namespace ra::mod::calculator {

CalculatorCommand::CalculatorCommand(
    const calculator::EvaluateResult& evaluate_result,
    const calculator::Modifier& modifier)
    :
    evaluate_result_(evaluate_result),
    modifier_(modifier) {

}


std::wstring CalculatorCommand::GetKeyword() {
    return {};
}


help::HelpContent CalculatorCommand::GetHelpContent() {
    constexpr const wchar_t* HelpContentID = L"calc";
    return {
        HelpContentID,
        help::BuiltInHelpContentManager::Instance().GetDetail(HelpContentID)
    };
}


bool CalculatorCommand::Interpret(
    const utility::CommandLine& command_line, 
    const context::DesktopContext& desktop_context,
    bool is_reusing) {

    //Not allow to reuse.
    if (is_reusing) {
        return false;
    }
    return true;
}


std::shared_ptr<CommandPreviewControl> CalculatorCommand::GetPreviewControl() {

    auto control = zaf::Create<CalculatorPreviewControl>();
    control->SetResult(evaluate_result_, modifier_);
    return control;
}


std::shared_ptr<CommandExecutor> CalculatorCommand::GetExecutor() {

    ResultTextBuilder text_builder(evaluate_result_, modifier_);
    return CopyExecutor::TryCreate(text_builder.Build().GetCompleteText());
}

}