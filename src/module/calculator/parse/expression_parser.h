#pragma once

#include "module/calculator/parse/non_terminal_parser.h"

namespace calculator {

class ExpressionParser : public NonTerminalParser {
public:
    static ExpressionParser* Instance();

protected:
    void InitializeParsers() override;

private:
    ExpressionParser() = default;
};

}