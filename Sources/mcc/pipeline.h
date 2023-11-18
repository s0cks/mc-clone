#ifndef MCC_PIPELINE_H
#define MCC_PIPELINE_H

#include <vector>
#include "mcc/gfx.h"

namespace mcc {
  class Pipeline {
  protected:
    Pipeline* parent_;
    std::vector<Pipeline*> children_;

    explicit Pipeline(Pipeline* parent = nullptr):
      parent_(nullptr),
      children_() {
    }

    void RenderChildren() {
      for(const auto& child : children_)
        child->Render();
    }
  public:
    virtual ~Pipeline() {
      for(const auto& child : children_)
        delete child;
      children_.clear();
      parent_ = nullptr;
    }

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
  };

  class ApplyPipeline : public Pipeline {
  protected:
    std::function<void()> apply_;
  public:
    ApplyPipeline(std::function<void()> apply):
      Pipeline(),
      apply_(apply) {
    }
    ~ApplyPipeline() override = default;

    void Render() override {
      apply_();
    }
  };

  template<typename Mesh>
  class RenderMeshPipeline : public Pipeline {
  private:
    Mesh* mesh_;
    bool children_first_;
  public:
    explicit RenderMeshPipeline(Mesh* mesh,
                                const bool children_first = true):
      Pipeline(),
      mesh_(mesh),
      children_first_(children_first) {
    }
    ~RenderMeshPipeline() override = default;

    inline Mesh* mesh() const {
      return mesh_;
    }

    void Render() override {
      if(children_first_) {
        RenderChildren();
        mesh()->Render();
      } else {
        mesh()->Render();
        RenderChildren();
      }
    }
  };
}

#endif //MCC_PIPELINE_H