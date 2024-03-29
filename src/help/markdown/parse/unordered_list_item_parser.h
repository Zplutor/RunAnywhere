#pragma once

#include "help/markdown/parse/list_item_parser.h"

namespace ra::help::markdown::parse {

class UnorderedListItemParser : public ListItemParser {
public:
    UnorderedListItemParser();
    explicit UnorderedListItemParser(wchar_t identity_char);

    wchar_t IdentityChar() const {
        return *identity_char_;
    }

protected:
    bool ParseItemIdentity(ParseContext& context) override;

private:
    std::optional<wchar_t> identity_char_;
};

}