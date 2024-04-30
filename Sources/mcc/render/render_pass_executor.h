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
  public:
    static inline bool
    Execute(RenderPass* pass) {
      RenderPassExecutor executor;
      return pass->Accept(&executor);
    }
  };
}

#endif //MCC_RENDER_PASS_EXECUTOR_H