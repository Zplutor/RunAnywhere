#include "module/crypto/crypto_module.h"
#include "module/crypto/md5/md5_command.h"
#include "module/crypto/md5/md5_command_parsing.h"

namespace ra::module::crypto {

std::shared_ptr<Command> CryptoModule::Interpret(const utility::CommandLine& command_line) {

	auto md5_parse_result = ParseMD5Command(command_line);
	if (md5_parse_result) {
		return std::make_shared<MD5Command>(*md5_parse_result);
	}

	return nullptr;
}

}