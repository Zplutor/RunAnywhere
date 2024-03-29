#pragma once

#include "module/tool/hash/hash_command.h"
#include "module/tool/hash/sha_algorithm_type.h"

namespace ra::mod::tool::hash {

class SHACommand : public HashCommand {
public:
    static std::wstring Keyword();

public:
    std::wstring GetKeyword() override;
    help::HelpContent GetHelpContent() override;

private:
    HashCommandParseResult ParseCommand(const utility::CommandLine& command_line) override;
    HashAlgorithmInfo GetHashAlgorithmInfo() override;

private:
    std::optional<SHAAlgorithmType> sha_algorithm_type;
};

}