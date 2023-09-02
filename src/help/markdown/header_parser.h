#pragma once

#include "help/markdown/style_parser.h"

namespace ra::help::markdown {

class HeaderParser : public StyleParser {
public:
    ParseStatus Parse(ParseContext& context, StyledStringBuilder& style_builder) override;
};

}