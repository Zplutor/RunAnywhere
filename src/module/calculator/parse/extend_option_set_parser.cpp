#include "module/calculator/parse/extend_option_set_parser.h"
#include <cctype>
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/string/to_numeric.h>
#include "module/calculator/parse/terminal_parser.h"

using namespace std::literals;

namespace ra::mod::calculator {
namespace {

class UnitOptionParser : public TerminalParser {
public:
    ParseStatus Parse(ParseContext& context, ParseResult& result) override {

        auto reader = context.BeginRead();
        if (reader.GetChar() != L'\'') {
            return ParseStatus::Mismatched;
        }

        reader.Forward();

        auto unit = GetNumberUnitByChar(reader.GetChar());
        if (unit) {

            result.GetMutableModifier().unit = *unit;

            reader.Forward();
            return ParseStatus::Ok;
        }

        return ParseStatus::Error;
    }
};


class HighlightOptionParser : public TerminalParser {
public:
    ParseStatus Parse(ParseContext& context, ParseResult& result) override {

        auto reader = context.BeginRead();
        if (reader.GetChar() != L'^') {
            return ParseStatus::Mismatched;
        }

        reader.Forward();

        static const auto digits = L"0123456789"s;

        std::wstring number;
        do {

            auto ch = reader.GetChar();
            if (zaf::Contain(digits, ch)) {
                number.append(1, ch);
            }
            else {
                break;
            }
        } 
        while (reader.Forward());

        int value{};
        if (!zaf::TryToNumeric<int>(number, value)) {
            return ParseStatus::Error;
        }

        result.GetMutableModifier().highlight_bit = value;
        return ParseStatus::Ok;
    }
};

}

ExtendOptionSetParser* ExtendOptionSetParser::Instance() {

    static ExtendOptionSetParser instance;
    return &instance;
}


ParseStatus ExtendOptionSetParser::Parse(ParseContext& context, ParseResult& result) {

    static UnitOptionParser unit_option_parser;
    static HighlightOptionParser highlight_option_parser;

    std::initializer_list<Parser*> option_parsers{
        &unit_option_parser, 
        &highlight_option_parser 
    };

    while (context.GetCurrentIndex() < context.GetLength()) {

        std::size_t mismatched_count{};

        for (auto each_parser : option_parsers) {

            auto parse_result = each_parser->Parse(context, result);
            if (parse_result == ParseStatus::Error) {
                return parse_result;
            }

            if (parse_result == ParseStatus::Mismatched) {
                mismatched_count++;
            }
        }

        if (mismatched_count == option_parsers.size()) {
            break;
        }

        context.SkipWhiteSpaces();
    }

    return ParseStatus::Ok;
}

}