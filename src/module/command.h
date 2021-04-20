#pragma once

#include <string>
#include "context/desktop_context.h"
#include "module/command_preview_control.h"

class Command {
public:
    Command() = default;
    virtual ~Command() = default;

    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

    const DesktopContext& GetDesktopContext() const {
        return desktop_context_;
    }

    void SetDesktopContext(const DesktopContext& desktop_context) {
        desktop_context_ = desktop_context;
    }

    virtual std::wstring GetPreviewText() {
        return {};
    }

    virtual std::shared_ptr<CommandPreviewControl> GetPreviewControl() {
        return nullptr;
    }

    virtual void Execute() = 0;

private:
    DesktopContext desktop_context_;
};