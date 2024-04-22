#ifndef MCC_RENDER_PASS_2D_H
#define MCC_RENDER_PASS_2D_H

#include "mcc/renderer/render_pass.h"

namespace mcc::render {
  class RenderPass2d : public SequenceRenderPass {
  public:
    RenderPass2d() = default;
    ~RenderPass2d() override = default;

    const char* GetName() const override {
      return "2d";
    }
  };
}

#endif //MCC_RENDER_PASS_2D_H