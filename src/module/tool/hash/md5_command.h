#pragma once

#include "module/tool/hash/hash_command.h"

namespace ra::mod::tool::hash {

class MD5Command : public HashCommand {
public:
    static CommandBrief Brief();

public:
    CommandBrief GetBrief() override;
    help::content::Content GetHelpContent() override;

private:
    HashAlgorithmCreator GetHashAlgorithmCreator();
};

}