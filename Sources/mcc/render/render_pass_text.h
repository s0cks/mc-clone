#ifndef MCC_RENDER_PASS_TEXT_H
#define MCC_RENDER_PASS_TEXT_H

#include "mcc/program/program.h"
#include "mcc/render/render_pass.h"

namespace mcc::render {
  class TextRenderPass : public RenderPass { //TODO: rename
  protected:
    Program* program_;
  public:
    TextRenderPass();
    ~TextRenderPass() override = default;

    bool Accept(RenderPassVisitor* vis) override {
      return vis->Visit(this);
    }

    bool ShouldSkip() const override;
    void Render() override;
    DECLARE_RENDER_PASS(Text);
  };
}

#endif //MCC_RENDER_PASS_GUIS_H