#pragma once

#include "context/discover/discoverer.h"

namespace ra::context {

class VSCodeDiscoverer : public Discoverer {
public:
    VSCodeDiscoverer();

    std::optional<ActivePath> Discover(
        const ForegroundWindowInfo& foreground_window_info) override;

private:
    static bool IsVSCodeProcess(DWORD process_id);
    static ActivePath GetActivePathFromVSCode();
};

}