#ifndef MCC_PIPELINE_H
#define MCC_PIPELINE_H

#include <vector>
#include "mcc/gfx.h"
#include "mcc/bitfield.h"
#include "mcc/framebuffer.h"
#include "mcc/shader/shader.h"
#include "mcc/material.h"

namespace mcc::pipeline {
  class Pipeline {
  protected:
    Pipeline* parent_;
    std::vector<Pipeline*> children_;
    glm::mat4 model_;
    ShaderRef shader_;
    MaterialRef material_;

    explicit Pipeline(Pipeline* parent = nullptr,
                      const glm::mat4 model = glm::mat4(1.0f)):
      parent_(nullptr),
      children_(),
      model_(model),
      shader_(),
      material_() {
    }
  public:
    virtual ~Pipeline() = default;
    virtual void Render() = 0;

    void SetParent(Pipeline* pipeline) {
      parent_ = pipeline;
    }

    Pipeline* GetParent() const {
      return parent_;
    }

    bool HasParent() const {
      return parent_ != nullptr;
    }

    bool HasChildren() const {
      return !children_.empty();
    }

    uint64_t GetNumberOfChildren() const {
      return children_.size();
    }

    void AddChild(Pipeline* pipeline) {
      children_.push_back(pipeline);
    }

    Pipeline* GetChildAt(const uint64_t idx) const {
      return children_[idx];
    }

    void SetChildAt(const uint64_t idx, Pipeline* pipeline) {
      children_[idx] = pipeline;
    }

    void SetMaterial(const MaterialRef& material) {
      material_ = material;
    }

    MaterialRef GetMaterial() const {
      return material_;
    }

    void SetShader(const ShaderRef& shader) {
      shader_ = shader;
    }

    ShaderRef GetShader() const {
      return shader_;
    }

    glm::mat4 GetModelMatrix() const {
      return model_;
    }

    void SetModelMatrix(const glm::mat4 model) {
      model_ = model;
    }
  };

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