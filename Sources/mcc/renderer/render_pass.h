#ifndef MCC_RENDER_PASS_H
#define MCC_RENDER_PASS_H

#include <cstdint>
#include "mcc/common.h"

namespace mcc {
  class RenderPass {
  protected:
    RenderPass() = default;
  public:
    virtual ~RenderPass() = default;
    virtual uint32_t order() const = 0;
    virtual const char* name() const = 0;
    virtual void Render() = 0;
  };

  template<const uint32_t Order>
  class RenderPassTemplate : public RenderPass {
  protected:
    RenderPassTemplate() = default;
  public:
    ~RenderPassTemplate() override = default;

    uint32_t order() const override {
      return Order;
    }
  };

  class RenderPassVisitor {
  protected:
    RenderPassVisitor() = default;
  public:
    virtual ~RenderPassVisitor() = default;
    virtual bool Visit(RenderPass* pass) = 0;
  };

  class RenderPassList {
    struct Node {
      Node* next;
      Node* previous;
      RenderPass* pass;

      Node() = default;
      explicit Node(RenderPass* p):
        next(nullptr),
        previous(nullptr),
        pass(p) {  
      }
      ~Node() {
        if(next)
          delete next;
      }

      inline uint32_t order() const {
        return pass->order();
      }
    };
  protected:
    Node* root_;

    inline Node* root() const {
      return root_;
    }

    inline bool HasRoot() const {
      return root() != nullptr;
    }

    inline void SetRoot(Node* node) {
      root_ = node;
    }
  public:
    RenderPassList():
      root_(nullptr) {
    }
    virtual ~RenderPassList() {
      delete root_;
    }

    virtual void Append(RenderPass* pass);
    virtual bool Visit(RenderPassVisitor* vis);
    virtual bool Visit(std::function<bool(RenderPass*)> vis);
  };
}

#endif //MCC_RENDER_PASS_H