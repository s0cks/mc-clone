#ifndef MCC_SCENE_H
#define MCC_SCENE_H

#include <vector>
#include <algorithm>
#include <glog/logging.h>

#include "mcc/common.h"
#include "mcc/shader/shader.h"
#include "mcc/rotation.h"
#include "mcc/fixed_rate_loop.h"

namespace mcc::mesh {
  class Mesh;
}

namespace mcc::scene {
  class Node;

#define DEFINE_SCENE_NODE(Name)                           \
  DEFINE_NON_COPYABLE_TYPE(Name##Node);                   \
  public:                                                 \
    Name##Node* As##Name##Node() override { return this; }         \
    const char* GetName() const override { return #Name; } \
    bool Visit(NodeVisitor* vis) override;

#define FOR_EACH_SCENE_NODE(V) \
  V(Window)                    \
  V(Model)                     \
  V(Shader)                    \
  V(Mesh)

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
    bool Is##Name##Node() { return As##Name##Node() != nullptr; }
    FOR_EACH_SCENE_NODE(DEFINE_NODE_TYPECHECK)
#undef DEFINE_NODE_TYPECHECK

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
    bool VisitShader(ShaderNode* node) override;
    bool VisitMesh(MeshNode* node) override;
  public:
    static inline void
    Render(Node* node) {
      NodeRenderer renderer;
      LOG_IF(ERROR, !node->Visit(&renderer)) << "failed to render " << node->GetName();
    }
  };

  class NodeUpdater : public NodeVisitor {
  protected:
    FixedRateLoop* loop_;

    explicit NodeUpdater(FixedRateLoop* loop):
      NodeVisitor(),
      loop_(loop) {
    }

    bool DoUpdates(Node* node) {
      const auto rate = (NANOSECONDS_PER_SECOND / TICKS_PER_SECOND);
      auto tick_time = (uint64_t)loop_->dms_ + (uint64_t)loop_->remainder_;
      VLOG(3) << "tick_time: " << tick_time << "/" << rate;
      while(tick_time >= rate) {
        if(!node->Visit(this))
          return false;
        loop_->ticks_ += 1;
        tick_time -= rate; 
      }
      loop_->remainder_ = std::max(tick_time, static_cast<uint64_t>(0));
      return true;
    }
  public:
    ~NodeUpdater() override = default;
    bool VisitWindow(WindowNode* node) override;
    bool VisitModel(ModelNode* node) override;
    bool VisitShader(ShaderNode* node) override;
    bool VisitMesh(MeshNode* node) override;

    FixedRateLoop* GetLoop() const {
      return loop_;
    }
  public:
    static inline void
    Update(FixedRateLoop* loop, Node* node) {
      NodeUpdater updater(loop);
      LOG_IF(ERROR, !updater.DoUpdates(node)) << "failed to update " << node->GetName();
    }
  };

  class WindowNode : public ContainerNode {
  protected:
    WindowNode() = default;
  public:
    ~WindowNode() override = default;
    DEFINE_SCENE_NODE(Window);
  };

  class ShaderNode : public Node {
  protected:
    Shader shader_;
  public:
    ShaderNode(const Shader& shader):
      Node(),
      shader_(shader) {
    }
    ~ShaderNode() override = default;
    DEFINE_SCENE_NODE(Shader);

    virtual Shader GetShader() const {
      return shader_;
    }

    ModelNode* GetModelNode() const {
      return GetParent()->AsModelNode();
    }
  };

  class MeshNode : public Node {
  protected:
    mesh::Mesh* mesh_;
  public:
    MeshNode(mesh::Mesh* mesh):
      Node(),
      mesh_(mesh) {
    }
    ~MeshNode() override = default;
    DEFINE_SCENE_NODE(Mesh);

    virtual mesh::Mesh* GetMesh() const {
      return mesh_;
    }

    ModelNode* GetModelNode() const {
      return GetParent()->AsModelNode();
    }
  };

  class NodeUpdater;
  class ModelNode : public Node {
    friend class NodeUpdater;
  public:
    static constexpr const glm::mat4 kIdentityMatrix = glm::mat4(1.0f);
  protected:
    glm::mat4 matrix_;
    ShaderNode* shader_;
    MeshNode* mesh_;
    Rotation rotation_;

    ModelNode():
      Node(),
      matrix_(kIdentityMatrix),
      shader_(nullptr),
      mesh_(nullptr),
      rotation_(1.0f, glm::vec3(1.0f, 0.0f, 0.0f)) {
    }

    void SetShaderNode(ShaderNode* node) {
      shader_ = node;
    }

    void SetMeshNode(MeshNode* node) {
      mesh_ = node;
    }

    void Update(NodeUpdater* updater);
  public:
    ~ModelNode() override = default;
    DEFINE_SCENE_NODE(Model);

    virtual glm::mat4 GetModelMatrix() const {
      return rotation_.Apply(matrix_);
    }

    ShaderNode* GetShaderNode() const {
      return shader_;
    }

    Shader GetShader() const {
      return GetShaderNode()->GetShader();
    }

    MeshNode* GetMeshNode() const {
      return mesh_;
    }

    mesh::Mesh* GetMesh() const {
      return GetMeshNode()->GetMesh();
    }

    bool VisitChildren(NodeVisitor* vis) const override {
      if(!GetShaderNode()->Visit(vis))
        return false;
      if(!GetMeshNode()->Visit(vis))
        return false;
      return true;
    }
  };
}

#endif //MCC_SCENE_H