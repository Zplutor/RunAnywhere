#pragma once

#include "module/command.h"
#include "module/command_brief.h"
#include "module/tool/date/date_command_parse_result.h"
#include "module/tool/date/date_preview_control.h"
#include "utility/command_line.h"

namespace ra::module::tool::date {

class DateCommand : public Command {
public:
    static CommandBrief GetBrief();

    static std::optional<DateCommandParseResult> Parse(const utility::CommandLine& command_line);

public:
    explicit DateCommand(const utility::CommandLine& command_line);

    help::content::Content GetHelpContent() override;
    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    void Execute() override;

private:
    std::optional<DateCommandParseResult> parse_result_;
    std::shared_ptr<DatePreviewControl> preview_control_;
};

}