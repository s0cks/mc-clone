#ifndef MCC_FBO_PIPELINE_H
#define MCC_FBO_PIPELINE_H

#include "mcc/color.h"
#include "mcc/fbo/fbo.h"
#include "mcc/pipeline.h"

#include "mcc/program/program.h"

namespace mcc {
  namespace fbo {
    class FboPipeline : public Pipeline {
    protected:
      const Fbo* fbo_;

      explicit FboPipeline(const Fbo* fbo):
        Pipeline(),
        fbo_(fbo) {
      }
    public:
      ~FboPipeline() override = default;

      const Fbo* GetFbo() const {
        return fbo_;
      }
    };

    class RenderFboPipeline : public FboPipeline {
    protected:
      Color clear_color_;
      GLbitfield clear_flags_;
      ProgramRef shader_;

      explicit RenderFboPipeline(const Fbo* fbo):
        FboPipeline(fbo) {
      }

      bool Apply() override;
    public:
      ~RenderFboPipeline() override = default;
      
      const char* GetName() const override {
        return "RenderFbo";
      }
    public:
      static RenderFboPipeline* New(const Fbo* fbo);
    };
  }
  using fbo::FboPipeline;
}

#endif //MCC_FBO_PIPELINE_H