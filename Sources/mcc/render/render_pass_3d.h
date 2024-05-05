#ifndef MCC_RENDER_PASS_3D_H
#define MCC_RENDER_PASS_3D_H

#include "mcc/pipeline.h"
#include "mcc/render/render_pass.h"

namespace mcc::render {
  class RenderPass3d : public SequenceRenderPass {
  protected:
    Pipeline* pipeline_;

    void Render() override {
      LOG_IF(ERROR, !pipeline_->Apply()) << "failed to apply pipeline.";
    }
  public:
    explicit RenderPass3d(Pipeline* pipeline);
    ~RenderPass3d() override;

    const char* GetName() const override {
      return "3d";
    }
  };
}

#endif //MCC_RENDER_PASS_3D_H