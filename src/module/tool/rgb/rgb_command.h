#pragma once

#include "module/command.h"
#include "module/command_brief.h"
#include "module/tool/rgb/rgb_command_parse_result.h"
#include "module/tool/rgb/rgb_preview_control.h"
#include "utility/command_line.h"

namespace ra::module::tool::rgb {

class RGBCommand : public Command {
public:
    static CommandBrief GetBrief();

    static std::optional<RGBCommandParseResult> Parse(const utility::CommandLine& command_line);

public:
    explicit RGBCommand(const utility::CommandLine& command_line);

    help::content::Content GetHelpContent() override;
    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    void Execute() override;

private:
    std::shared_ptr<RGBPreviewControl> preview_control_;
};

}