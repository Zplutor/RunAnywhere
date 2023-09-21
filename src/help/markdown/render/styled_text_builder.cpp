#include "help/markdown/render/styled_text_builder.h"
#include <zaf/base/error/check.h>

namespace ra::help::markdown::render {

StyledText StyledTextBuilder::Build(
    const element::Element& element, 
    const StyleConfig& style_config) {

    TextStyle basic_style;
    basic_style.font = style_config.basic_config.font;

    StyledText result;
    BuildElement(element, basic_style, style_config, result);

    ZAF_EXPECT(result.CheckConstraint());
    return result;
}


void StyledTextBuilder::BuildElement(
    const element::Element& element, 
    const TextStyle& current_style,
    const StyleConfig& style_config,
    StyledText& result) {

    if (element.IsTextElement()) {
        result.Append(element.Text());
        return;
    }

    result.AddStyleToPendingText(current_style);

    auto new_style = CreateNewStyleByElement(element, current_style, style_config);

    for (const auto& each_child : element.Children()) {
        BuildElement(*each_child, new_style, style_config, result);
    }

    result.AddStyleToPendingText(new_style);
}


TextStyle StyledTextBuilder::CreateNewStyleByElement(
    const element::Element& element, 
    const TextStyle& basic_style,
    const StyleConfig& style_config) {

    auto new_style = basic_style;

    switch (element.Type()) {

    case element::ElementType::Paragraph:
        //No need to change style of paragraph.
        break;

    case element::ElementType::Bold:
        new_style.font.weight = style_config.bold_font_weight;
        break;

    case element::ElementType::InlineCode:
        new_style.font.family_name = style_config.code_config.font_family_name;
        break;

    default:
        ZAF_NOT_REACHED();
    }

    return new_style;
}

}