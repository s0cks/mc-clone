#ifndef MCC_RENDER_PASS_GUIS_H
#define MCC_RENDER_PASS_GUIS_H

#include "mcc/gui/gui_component.h"
#include "mcc/renderer/render_pass.h"

namespace mcc::render {
  class RenderPassGuis : public RenderPass {
  public:
    RenderPassGuis() = default;
    ~RenderPassGuis() override = default;
    bool ShouldSkip() const override;
    DECLARE_RENDER_PASS(Guis);

    void Render() override;
  };
}

#endif //MCC_RENDER_PASS_GUIS_H