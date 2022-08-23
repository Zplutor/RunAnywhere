#pragma once

#include <filesystem>

namespace ra::module::user_defined {

bool UpgradeLegacyEntries(
    const std::filesystem::path& legacy_file_path, 
    const std::filesystem::path& bundle_file_path);

}