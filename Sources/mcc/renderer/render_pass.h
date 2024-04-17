#ifndef MCC_RENDER_PASS_H
#define MCC_RENDER_PASS_H

#include <cstdint>
#include "mcc/common.h"

namespace mcc::render {
  class RenderPass;
  class RenderPassVisitor {
  protected:
    RenderPassVisitor() = default;
  public:
    virtual ~RenderPassVisitor() = default;
    virtual bool Visit(RenderPass* pass) = 0;
  };

  class RenderPass {
  protected:
    RenderPass() = default;
  public:
    virtual ~RenderPass() = default;
    virtual const char* GetName() const = 0;
    virtual void Append(RenderPass* pass) = 0;
    virtual void Render() = 0;
    virtual bool Accept(RenderPassVisitor* vis) = 0;
    virtual bool HasChildren() const = 0;
    virtual uint32_t GetNumberOfChildren() const = 0;
    virtual RenderPass* GetChildAt(const uint32_t idx) const = 0;
  };

  class RenderPassSequence : public RenderPass {
  protected:
    std::vector<RenderPass*> children_;

    void Render() override { }
  public:
    RenderPassSequence():
      RenderPass() {
    }
    ~RenderPassSequence() override = default;

    const char* GetName() const override {
      return "Sequence";
    }

    bool HasChildren() const override {
      return !children_.empty();
    }

    uint32_t GetNumberOfChildren() const override {
      return children_.size();
    }

    RenderPass* GetChildAt(const uint32_t idx) const override {
      MCC_ASSERT(idx >= 0 && idx <= children_.size());
      return children_[idx];
    }

    bool Accept(RenderPassVisitor* vis) override {
      for(const auto& child : children_) {
        if(!vis->Visit(child))
          return false;
      }
      return true;
    }

    void Append(RenderPass* pass) override {
      children_.push_back(pass);
    }
  };
}

#endif //MCC_RENDER_PASS_H