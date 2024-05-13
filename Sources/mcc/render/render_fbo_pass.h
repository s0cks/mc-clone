#ifndef MCC_RENDER_FBO_PASS_H
#define MCC_RENDER_FBO_PASS_H

#include "mcc/fbo/fbo.h"
#include "mcc/fbo/fbo_scope.h"
#include "mcc/program/program.h"
#include "mcc/render/render_pass.h"

namespace mcc::render {
  class RenderFboPass : public RenderPass {
  protected:
    Fbo* fbo_;
    Program* program_; 

    void Render() override;
  public:
    explicit RenderFboPass(Fbo* fbo):
      RenderPass(),
      fbo_(fbo),
      program_(Program::FromJson("program:textured_2d")) {
    }
    ~RenderFboPass() override = default;

    Order GetOrder() const override {
      return kLast;
    }

    const char* GetName() const override {
      return "RenderFbo";
    }
  };
}

#endif //MCC_RENDER_FBO_PASS_H