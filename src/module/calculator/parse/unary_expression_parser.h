#pragma once

#include "module/calculator/parse/non_terminal_parser.h"

namespace ra::mod::calculator {

class UnaryExpressionParser : public NonTerminalParser {
public:
    static UnaryExpressionParser* Instance();

protected:
    void InitializeParsers() override;

private:
    UnaryExpressionParser() = default;
};

}