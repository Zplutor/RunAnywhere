#pragma once

#include "module/calculator/parse/non_terminal_parser.h"

namespace ra::mod::calculator {

class CommonOptionSetParser : public NonTerminalParser {
public:
    static CommonOptionSetParser* Instance();

protected:
    void InitializeParsers() override;

private:
    CommonOptionSetParser() = default;
};

}