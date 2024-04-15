#include "mcc/renderer/render_pass.h"

namespace mcc {
  void RenderPassList::Append(RenderPass* pass) {
    if(!HasRoot()) {
      SetRoot(new Node(pass));
      return;
    }

    auto node = root_;
    while(node->next && node->next->order() <= pass->order()) {
      node = node->next;
    }
    MCC_ASSERT(node);

    const auto& next = node->next;
    if(next) {
      MCC_ASSERT(next->order() > pass->order());
      const auto new_node = new Node(pass);
      next->previous = new_node;
      node->next = new_node;
      return;
    }

    const auto new_node = new Node(pass);
    new_node->previous = node;
    node->next = new_node;
  }

#define FOREACH_NODE(Name)    \
  auto Name = root_;          \
  while(Name != nullptr)

  bool RenderPassList::Visit(RenderPassVisitor* vis) {
    FOREACH_NODE(node) {
      if(!vis->Visit(node->pass))
        return false;
      node = node->next;
    }
    return false;
  }

  bool RenderPassList::Visit(std::function<bool(RenderPass*)> vis) {
    FOREACH_NODE(node) {
      if(!vis(node->pass))
        return false;
      node = node->next;
    }
    return false;
  }
}