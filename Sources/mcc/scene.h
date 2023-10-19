#ifndef MCC_SCENE_H
#define MCC_SCENE_H

#include <vector>
#include <glog/logging.h>

#include "mcc/common.h"
#include "mcc/shader/shader.h"

namespace mcc {
  class Mesh;
}

namespace mcc::scene {
  class Node;

#define DEFINE_SCENE_NODE(Name)                           \
  DEFINE_NON_COPYABLE_TYPE(Name##Node);                   \
  public:                                                 \
    Name##Node* As##Name##Node() { return this; }         \
    const char* GetName() const override { return #Name; } \
    bool Visit(NodeVisitor* vis) override;

#define FOR_EACH_SCENE_NODE(V) \
  V(Window)                    \
  V(Model)

#define FORWARD_DECLARE(Name) class Name##Node;
  FOR_EACH_SCENE_NODE(FORWARD_DECLARE);
#undef FORWARD_DECLARE

  class NodeVisitor;
  class Node {
  protected:
    Node* parent_;

    Node() = default;
  public:
    virtual ~Node() = default;
    virtual bool Visit(NodeVisitor* vis) = 0;
    virtual bool VisitChildren(NodeVisitor* vis) const { return true; }
    virtual uint64_t GetNumberOfChildren() const { return 0; }
    virtual const char* GetName() const = 0;

#define DEFINE_NODE_TYPECHECK(Name) \
    virtual Name##Node* As##Name##Node() { return nullptr; } \
    bool Is##Name##Node() const { return As##Name##Node() != nullptr; }

    Node* GetParent() const {
      return parent_;
    }

    bool HasParent() const {
      return GetParent() != nullptr;
    }

    bool IsRoot() const {
      return GetParent() == nullptr;
    }

    bool IsLeaf() const {
      return GetNumberOfChildren() == 0;
    }
  };

  class NodeVisitor {
  protected:
    NodeVisitor() = default;
  public:
    virtual ~NodeVisitor() = default;
#define DECLARE_VISIT_FUNCTION(Name) \
    virtual bool Visit##Name(Name##Node* node) = 0;
    FOR_EACH_SCENE_NODE(DECLARE_VISIT_FUNCTION)
#undef DECLARE_VISIT_FUNCTION
  };

  class ContainerNode : public Node {
  protected:
    std::vector<Node*> children_;

    ContainerNode():
      Node(),
      children_() {
    }
  public:
    ~ContainerNode() override = default;

    uint64_t GetNumberOfChildren() const override {
      return children_.size();
    }

    void AppendChild(Node* node) {
      children_.push_back(node);
    }

    bool VisitChildren(NodeVisitor* vis) const override {
      for(auto it = children_.begin();
          it != children_.end();
          it++) {
          if(!(*it)->Visit(vis))
            return false;
      }
      return true;
    }
  };

  class NodeRenderer : public NodeVisitor {
  protected:
    NodeRenderer() = default;

    void RenderNode(Node* node);
  public:
    ~NodeRenderer() override = default;

    bool VisitWindow(WindowNode* node) override;
    bool VisitModel(ModelNode* node) override;
  public:
    static inline void
    Render(Node* node) {
      NodeRenderer renderer;
      LOG_IF(ERROR, !node->Visit(&renderer)) << "failed to render " << node->GetName();
    }
  };

  class WindowNode : public ContainerNode {
  protected:
    WindowNode() = default;
  public:
    ~WindowNode() override = default;
    DEFINE_SCENE_NODE(Window);
  };

  class ModelNode : public Node {
  protected:
    ModelNode() = default;
  public:
    ~ModelNode() override = default;
    virtual Shader GetShader() const = 0;
    virtual Mesh* GetMesh() const = 0;
    DEFINE_SCENE_NODE(Model);
  };
}

#endif //MCC_SCENE_H