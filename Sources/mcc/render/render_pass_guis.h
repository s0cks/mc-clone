#ifndef MCC_RENDER_PASS_GUIS_H
#define MCC_RENDER_PASS_GUIS_H

#include "mcc/gui/gui_component.h"
#include "mcc/render/render_pass.h"

#include "mcc/program/program.h"
#include "mcc/camera/camera_ortho.h"

namespace mcc::render {
  class RenderPassGuis : public RenderPass {
  protected:
    ProgramRef prog_;
  public:
    RenderPassGuis();
    ~RenderPassGuis() override = default;

    bool Accept(RenderPassVisitor* vis) override {
      return vis->Visit(this);
    }

    bool ShouldSkip() const override;
    void Render() override;
    DECLARE_RENDER_PASS(Guis);
  };
}

#endif //MCC_RENDER_PASS_GUIS_H