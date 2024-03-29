#include "module/tool/hash/sha_command.h"
#include <zaf/crypto/sha.h>
#include "help/built_in_help_content_manager.h"
#include "module/tool/hash/sha_algorithm_type_parsing.h"

namespace ra::mod::tool::hash {

std::wstring SHACommand::Keyword() {
	return L"sha";
}


std::wstring SHACommand::GetKeyword() {
	return Keyword();
}


help::HelpContent SHACommand::GetHelpContent() {
	return {
		Keyword(),
		help::BuiltInHelpContentManager::Instance().GetDetail(Keyword()) 
	};
}


HashCommandParseResult SHACommand::ParseCommand(const utility::CommandLine& command_line) {

	sha_algorithm_type = ParseSHAAlgorithmType(command_line);
	return __super::ParseCommand(command_line);
}


HashAlgorithmInfo SHACommand::GetHashAlgorithmInfo() {

	HashAlgorithmInfo result;

	switch (sha_algorithm_type.value_or(SHAAlgorithmType::SHA256)) {
	case SHAAlgorithmType::SHA1:
		result.displayed_name = L"1";
		result.tooltip = L"SHA1 algorithm";
		result.algorithm_creator = []() { return zaf::crypto::SHA1{}; };
		break;
	case SHAAlgorithmType::SHA256:
		result.displayed_name = L"256";
		result.tooltip = L"SHA256 algorithm";
		result.algorithm_creator = []() { return zaf::crypto::SHA256{}; };
		break;
	case SHAAlgorithmType::SHA384:
		result.displayed_name = L"384";
		result.tooltip = L"SHA384 algorithm";
		result.algorithm_creator = []() { return zaf::crypto::SHA384{}; };
		break;
	case SHAAlgorithmType::SHA512:
		result.displayed_name = L"512";
		result.tooltip = L"SHA512 algorithm";
		result.algorithm_creator = []() { return zaf::crypto::SHA512{}; };
		break;
	default:
		ZAF_NOT_REACHED();
	}

	return result;
}

}