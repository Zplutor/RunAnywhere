#pragma once

#include "help/markdown/render/render_region.h"

namespace ra::help::markdown::render {

class CompoundRegion : public RenderRegion {
protected:
    explicit CompoundRegion(std::vector<std::shared_ptr<RenderRegion>> child_regions);

    void Initialize() override;
    void Layout(const zaf::Rect&) override;
    zaf::Size CalculatePreferredContentSize(const zaf::Size& bound_size) const override;

private:
    std::vector<std::shared_ptr<RenderRegion>> child_regions_;
};

}