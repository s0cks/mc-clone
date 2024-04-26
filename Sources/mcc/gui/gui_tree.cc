#include "mcc/gui/gui_tree.h"

#include <vector>
#include "mcc/thread_local.h"

namespace mcc::gui {
  static rx::subject<RootComponentEvent*> events_;
  static std::vector<Component*> roots_;

  int32_t Tree::GetNumberOfRoots() {
    return roots_.size();
  }

  Component* Tree::GetRoot(const int32_t idx) {
    MCC_ASSERT(idx >= 0 && idx <= GetNumberOfRoots());
    return roots_[idx];
  }

  void Tree::AddRoot(Component* root) {
    MCC_ASSERT(root);
    roots_.push_back(root);
    Publish<RootComponentRegisteredEvent>(root);
  }

  void Tree::RemoveRoot(const int32_t idx) {
    MCC_ASSERT(idx >= 0 && idx <= GetNumberOfRoots());
    NOT_IMPLEMENTED(ERROR); //TODO: implement
  }

  void Tree::RemoveRoot(Component* root) {
    MCC_ASSERT(root);
    NOT_IMPLEMENTED(ERROR);//TODO: implement
  }

  void Tree::Publish(RootComponentEvent* event) {
    MCC_ASSERT(event);
    const auto& subscriber = events_.get_subscriber();
    subscriber.on_next(event);
  }

  rx::observable<Component*> Tree::GetAllRoots() {
    return rx::observable<>::iterate(roots_);
  }

  rx::observable<RootComponentEvent*> Tree::OnEvent() {
    return events_.get_observable();
  }
}