#include <gtest/gtest.h>
#include "utility/markdown/element/factory.h"
#include "utility/markdown/parse/bold_parser.h"

using namespace ra::utility::markdown::element;
using namespace ra::utility::markdown::parse;

TEST(BoldParserTest, Parse) {

    auto test = [](std::wstring_view input, ElementList expected) {
        ParseContext context(input);
        auto element = BoldParser::Instance()->Parse(context);
        if (!element) {
            return false;
        }
        return element->IsEqualTo(*MakeBold(std::move(expected)));
    };

    ASSERT_TRUE(test(L"**abc**", { MakeText(L"abc")}));
    ASSERT_TRUE(test(L"**`abc`**", { MakeInlineCode(L"abc") }));
}


TEST(BoldParserTest, Failure) {

    auto test = [](std::wstring_view input) {
        ParseContext context(input);
        auto element = BoldParser::Instance()->Parse(context);
        return !element && (context.CurrentIndex() == 0);
    };

    ASSERT_TRUE(test(L"abc"));
    ASSERT_TRUE(test(L"abc**"));
    ASSERT_TRUE(test(L"*abcd*"));
    ASSERT_TRUE(test(L"**abcd"));
    ASSERT_TRUE(test(L"**abcd*"));
    ASSERT_TRUE(test(L"** ab**"));
    ASSERT_TRUE(test(L"**ab **"));
    ASSERT_TRUE(test(L"** ab **"));
    ASSERT_TRUE(test(L"**ab\n**"));
    ASSERT_TRUE(test(L"***ab***"));
}