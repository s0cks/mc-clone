#ifndef MCC_PIPELINE_H
#define MCC_PIPELINE_H

#include "mcc/gfx.h"
#include "mcc/bitfield.h"
#include "mcc/framebuffer.h"
#include "mcc/shader/shader.h"
#include "mcc/material.h"

namespace mcc::pipeline {
  template<typename R>
  class RenderablePipeline {
    DEFINE_NON_COPYABLE_TYPE(RenderablePipeline);
  public:
    static constexpr const GLint kCameraSlot = 0;
    static constexpr const GLint kDirectionLightSlot = 1;
    static constexpr const GLint kPointLightSlot = 2;

    static constexpr const uint8_t kMaxNumberOfTextures = 32;
  private:
    const R& target_;
    glm::mat4 model_;
    shader::Shader shader_;
  public:
    RenderablePipeline(const R& target):
      target_(target),
      model_(1.0f) {
    }
    explicit RenderablePipeline(const R& target,
                                const glm::mat4 model):
      target_(target),
      model_(model) {
    }
    virtual ~RenderablePipeline() = default;

    const R& GetTarget() const {
      return target_;
    }

    void SetModelMatrix(const glm::mat4& value) {
      model_ = value;
    }

    glm::mat4 GetModelMatrix() const {
      return model_;
    }

    shader::Shader GetShader() const {
      return shader_;
    }

    void SetShader(const shader::Shader& value) {
      shader_ = value;
    }

    void Render() {
      shader_.ApplyShader();
      target_.Render();
    }
  };
}

#endif //MCC_PIPELINE_H