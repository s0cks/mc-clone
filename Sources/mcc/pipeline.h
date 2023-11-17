#ifndef MCC_PIPELINE_H
#define MCC_PIPELINE_H

#include <vector>
#include "mcc/gfx.h"

namespace mcc {
  template<typename I, typename O>
  class Pipeline {
  protected:
    Pipeline* parent_;
    std::vector<Pipeline*> children_;

    explicit Pipeline(Pipeline* parent = nullptr):
      parent_(nullptr),
      children_() {
    }
  public:
    virtual ~Pipeline() = default;
    virtual O Render(I) = 0;

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
}

#endif //MCC_PIPELINE_H