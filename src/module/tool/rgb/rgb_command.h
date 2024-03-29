#pragma once

#include "module/command.h"
#include "module/command_brief.h"
#include "module/tool/rgb/rgb_command_parse_result.h"
#include "module/tool/rgb/rgb_preview_control.h"
#include "utility/command_line.h"

namespace ra::mod::tool::rgb {

class RGBCommand : public Command {
public:
    static std::wstring Keyword();

    static std::optional<RGBCommandParseResult> Parse(const utility::CommandLine& command_line);

public:
    std::wstring GetKeyword() override;
    help::HelpContent GetHelpContent() override;
    bool Interpret(
        const utility::CommandLine& command_line, 
        const context::DesktopContext& desktop_context, 
        bool is_reusing) override;
    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    std::shared_ptr<CommandExecutor> GetExecutor() override;

private:
    std::optional<RGBCommandParseResult> parse_result_;
    std::shared_ptr<RGBPreviewControl> preview_control_;
};

}