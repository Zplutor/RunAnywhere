#pragma once

#include "module/command.h"
#include "module/command_brief.h"
#include "utility/command_line.h"
#include "module/tool/text_transform/text_preview_control.h"
#include "module/tool/text_transform/text_transformer.h"

namespace ra::module::tool::text_transform {

class TextTransformCommand : public Command {
public:
    explicit TextTransformCommand(std::unique_ptr<TextTransformer> transformer);

    CommandBrief GetBrief() override;
    help::content::Content GetHelpContent() override;
    bool Interpret(
        const utility::CommandLine& command_line,
        const context::DesktopContext& desktop_context,
        bool is_reusing) override;
    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    void Execute() override;

private:
    std::unique_ptr<TextTransformer> transformer_;
    std::wstring transformed_text_;
    std::shared_ptr<TextPreviewControl> preview_control_;
};

}