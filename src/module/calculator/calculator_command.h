#pragma once

#include <Windows.h>
#include "module/calculator/evaluate/evaluate_result.h"
#include "module/calculator/parse/modifier.h"
#include "module/command.h"

namespace ra::module::calculator {

class CalculatorCommand : public Command {
public:
    CalculatorCommand(
        const calculator::EvaluateResult& evaluate_result, 
        const calculator::Modifier& modifier);

    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    void Execute() override;

private:
    calculator::EvaluateResult evaluate_result_;
    calculator::Modifier modifier_;
};

}