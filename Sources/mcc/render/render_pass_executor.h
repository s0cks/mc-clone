#ifndef MCC_RENDER_PASS_EXECUTOR_H
#define MCC_RENDER_PASS_EXECUTOR_H

#include "mcc/render/render_pass.h"

namespace mcc::render {
  class RenderPassExecutor : public RenderPassVisitor {
  protected:
    bool Visit(RenderPass* pass) override;
  public:
    RenderPassExecutor() = default;
    ~RenderPassExecutor() override = default;
    virtual bool ExecuteRenderPass(RenderPass* pass);
  public:
    static inline bool
    Execute(RenderPass* pass) {
      RenderPassExecutor executor;
      return executor.ExecuteRenderPass(pass);
    }
  };
}

#endif //MCC_RENDER_PASS_EXECUTOR_H