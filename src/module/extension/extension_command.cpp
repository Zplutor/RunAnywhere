#include "module/extension/extension_command.h"
#include <zaf/creation.h>
#include "utility/clipboard.h"
#include "module/extension/extension_command_preview_control.h"

namespace ra::module::extension {

ExtensionCommand::ExtensionCommand(
    RunAnywhere_Command command, 
    const ExtensionInterface& extension_interface)
    : 
    command_(command),
    interface_(extension_interface) {

}


ExtensionCommand::~ExtensionCommand() {

    interface_.destroy_function(command_);
}


CommandBrief ExtensionCommand::GetBrief() {
    return CommandBrief{ L"", L"" };
}


bool ExtensionCommand::Interpret(
    const utility::CommandLine& command_line,
    const context::DesktopContext& desktop_context,
    bool is_reusing) {

    //Not allow to resuse.
    if (is_reusing) {
        return false;
    }
    return true;
}


std::shared_ptr<CommandPreviewControl> ExtensionCommand::GetPreviewControl() {

    auto control = zaf::Create<ExtensionCommandPreviewControl>();
    control->SetText(interface_.get_text_function(command_));
    return control;
}


void ExtensionCommand::Execute() {

    utility::SetStringToClipboard(GetPreviewText());
}

}