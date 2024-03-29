#include "module/tool/text_transform/upper_transformer.h"
#include <zaf/base/string/case_conversion.h>
#include "help/built_in_help_content_manager.h"

namespace ra::mod::tool::text_transform {

std::wstring UpperTransformer::Keyword() {
    return L"up";
}


std::wstring UpperTransformer::GetKeyword() {
    return Keyword();
}


help::HelpContent UpperTransformer::GetHelpContent() {
    return {
        Keyword(),
        help::BuiltInHelpContentManager::Instance().GetDetail(Keyword())
    };
}


void UpperTransformer::Transform(std::wstring & text) {
    zaf::Uppercase(text);
}

}