#ifndef MCC_SHADER_PIPELINE_H
#define MCC_SHADER_PIPELINE_H

#include "mcc/pipeline.h"
#include "mcc/shader/shader.h"

namespace mcc {
  namespace shader {
    typedef std::function<void(const ShaderRef&)> ApplyShaderFunction;

    class ApplyShaderPipeline : public Pipeline {
    private:
      ShaderRef shader_;
      ApplyShaderFunction apply_;
    public:
      explicit ApplyShaderPipeline(ShaderRef shader, ApplyShaderFunction apply):
        Pipeline(),
        shader_(shader),
        apply_(apply) {
      }
      ~ApplyShaderPipeline() override = default;

      ShaderRef shader() const {
        return shader_;
      }

      void Render() override {
        shader_->ApplyShader();
        apply_(shader_);
      }
    };
  }
  using shader::ApplyShaderPipeline;
}

#endif //MCC_SHADER_PIPELINE_H