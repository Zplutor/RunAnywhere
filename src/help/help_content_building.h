#pragma once

#include "help/markdown/element/element.h"
#include "module/command_brief.h"

namespace ra::help {

std::shared_ptr<markdown::element::Element> GetNoHelpContentParagraph();
std::shared_ptr<markdown::element::Element> BuildDescriptionParagraph(std::wstring description);

std::shared_ptr<markdown::element::Element> BuildHelpContentFromSuggestedCommands(
    std::vector<mod::CommandBrief>&& commands);

}