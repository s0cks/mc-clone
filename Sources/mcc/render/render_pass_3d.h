#ifndef MCC_RENDER_PASS_3D_H
#define MCC_RENDER_PASS_3D_H

#include "mcc/pipeline.h"
#include "mcc/render/render_pass.h"

namespace mcc::render {
  class RenderPass3d : public SequenceRenderPass {
  protected:
    Pipeline* pipeline_;

    void Render() override {
      pipeline_->Render();
    }
  public:
    explicit RenderPass3d(Pipeline* pipeline):
      SequenceRenderPass(),
      pipeline_(pipeline) {
    }
    ~RenderPass3d() override = default;

    const char* GetName() const override {
      return "3d";
    }
  };
}

#endif //MCC_RENDER_PASS_3D_H