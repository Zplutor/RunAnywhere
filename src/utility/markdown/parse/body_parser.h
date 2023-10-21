#pragma once

#include <zaf/base/non_copyable.h>
#include "utility/markdown/element/element.h"
#include "utility/markdown/parse/block_parser.h"
#include "utility/markdown/parse/code_block_parser.h"
#include "utility/markdown/parse/empty_line_info.h"
#include "utility/markdown/parse/header_parser.h"
#include "utility/markdown/parse/ordered_list_parser.h"
#include "utility/markdown/parse/paragraph_parser.h"
#include "utility/markdown/parse/parse_context.h"
#include "utility/markdown/parse/unordered_list_parser.h"

namespace ra::utility::markdown::parse {

class BodyParser : zaf::NonCopyableNonMovable {
public:
    struct Result {
        element::ElementList elements;
        EmptyLineInfo empty_line_info;
    };

public:
    void ParseOneLine(ParseContext& context);
    Result Finish();

    bool IsLastParagraphFinished() const {
        return is_last_paragraph_finished_;
    }

private:
    void InnerParseOneLine(ParseContext& context);
    bool ParseOneBlockLine(ParseContext& context, BlockParser::Result& block_result);
    void HandleBlockResult(BlockParser::Result&& block_result);

private:
    ParagraphParser paragraph_parser_;
    HeaderParser header_parser_;
    CodeBlockParser code_block_parser_;
    UnorderedListParser unordered_list_parser_;
    OrderedListParser ordered_list_parser_;

    BlockParser* current_block_parser_{};
    element::ElementList elements_;
    EmptyLineInfo empty_line_info_;
    bool is_last_paragraph_finished_{};
    bool is_body_finished_{};
};

}