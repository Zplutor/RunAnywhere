#include <gtest/gtest.h>
#include "module/calculator/parse/expression_parser.h"

using namespace ra::mod::calculator;

TEST(ExpressionParserTest, LeftToRightAssociativity) {

    ParseContext parse_context{ L"1-1-1-1" };
    ParseResult parse_result;
    auto parse_status = ExpressionParser::Instance()->Parse(parse_context, parse_result);
    ASSERT_EQ(parse_status, ParseStatus::Ok);

    //First level
    auto current_node = std::dynamic_pointer_cast<OperatorNode>(parse_result.GetExpressionRootNode());
    ASSERT_NE(current_node, nullptr);
    ASSERT_EQ(current_node->type, OperatorNode::Type::Minus);
    ASSERT_EQ(current_node->children.size(), 2);

    {
        auto second_child = std::dynamic_pointer_cast<OperandNode>(current_node->children[1]);
        ASSERT_NE(second_child, nullptr);
        ASSERT_EQ(second_child->text, L"1");

        auto first_child = std::dynamic_pointer_cast<OperatorNode>(current_node->children[0]);
        ASSERT_NE(first_child, nullptr);
        ASSERT_EQ(first_child->type, OperatorNode::Type::Minus);
        ASSERT_EQ(first_child->children.size(), 2);

        current_node = first_child;
    }

    //Second level
    {
        auto second_child = std::dynamic_pointer_cast<OperandNode>(current_node->children[1]);
        ASSERT_NE(second_child, nullptr);
        ASSERT_EQ(second_child->text, L"1");

        auto first_child = std::dynamic_pointer_cast<OperatorNode>(current_node->children[0]);
        ASSERT_NE(first_child, nullptr);
        ASSERT_EQ(first_child->type, OperatorNode::Type::Minus);
        ASSERT_EQ(first_child->children.size(), 2);

        current_node = first_child;
    }

    //Third level
    {
        auto second_child = std::dynamic_pointer_cast<OperandNode>(current_node->children[1]);
        ASSERT_NE(second_child, nullptr);
        ASSERT_EQ(second_child->text, L"1");

        auto first_child = std::dynamic_pointer_cast<OperandNode>(current_node->children[0]);
        ASSERT_NE(first_child, nullptr);
        ASSERT_EQ(second_child->text, L"1");
    }
}


TEST(ExpressionParserTest, Parenthesis) {

    {
        ParseContext parse_context{ L"(1)" };
        ParseResult parse_result;
        auto parse_status = ExpressionParser::Instance()->Parse(parse_context, parse_result);
        ASSERT_EQ(parse_status, ParseStatus::Ok);

        auto node = std::dynamic_pointer_cast<OperandNode>(parse_result.GetExpressionRootNode());
        ASSERT_NE(node, nullptr);
        ASSERT_EQ(node->text, L"1");
    }

    {
        ParseContext parse_context{ L"((2))" };
        ParseResult parse_result;
        auto parse_status = ExpressionParser::Instance()->Parse(parse_context, parse_result);
        ASSERT_EQ(parse_status, ParseStatus::Ok);

        auto node = std::dynamic_pointer_cast<OperandNode>(parse_result.GetExpressionRootNode());
        ASSERT_NE(node, nullptr);
        ASSERT_EQ(node->text, L"2");
    }

    {
        ParseContext parse_context{ L"(((3)))" };
        ParseResult parse_result;
        auto parse_status = ExpressionParser::Instance()->Parse(parse_context, parse_result);
        ASSERT_EQ(parse_status, ParseStatus::Ok);

        auto node = std::dynamic_pointer_cast<OperandNode>(parse_result.GetExpressionRootNode());
        ASSERT_NE(node, nullptr);
        ASSERT_EQ(node->text, L"3");
    }
}


TEST(ExpressionParserTest, ContinuousUnaryOperators) {

    ParseContext parse_context{ L"-+-1" };
    ParseResult parse_result;
    auto parse_status = ExpressionParser::Instance()->Parse(parse_context, parse_result);
    ASSERT_EQ(parse_status, ParseStatus::Ok);

    auto node0 = std::dynamic_pointer_cast<OperatorNode>(parse_result.GetExpressionRootNode());
    ASSERT_NE(node0, nullptr);
    ASSERT_EQ(node0->type, OperatorNode::Type::Negative);
    ASSERT_EQ(node0->children.size(), 1);

    auto node1 = std::dynamic_pointer_cast<OperatorNode>(node0->children.front());
    ASSERT_NE(node1, nullptr);
    ASSERT_EQ(node1->type, OperatorNode::Type::Positive);
    ASSERT_EQ(node1->children.size(), 1);

    auto node2 = std::dynamic_pointer_cast<OperatorNode>(node1->children.front());
    ASSERT_NE(node2, nullptr);
    ASSERT_EQ(node2->type, OperatorNode::Type::Negative);
    ASSERT_EQ(node2->children.size(), 1);

    auto node3 = std::dynamic_pointer_cast<OperandNode>(node2->children.front());
    ASSERT_NE(node3, nullptr);
    ASSERT_EQ(node3->text, L"1");
}