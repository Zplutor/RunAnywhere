#pragma once

#include <filesystem>
#include <zaf/control/control_binder.h>
#include "module/command_preview_control.h"
#include "module/tool/hex/hex_command_parse_result.h"
#include "module/tool/hex/hex_content_control.h"

namespace ra::module::tool::hex {

class HexPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

    void ShowFileContent(
        const std::filesystem::path& file_path,
        const HexCommandParseResult& parse_result);

private:
    ZAF_BIND_CONTROL(HexContentControl, contentControl);

    std::filesystem::path file_path_;
};

}