#include "mcc/ubo/ubo_registry.h"
#include "mcc/ubo/ubo.h"

namespace mcc::ubo {
  bool UboRegistry::Comparator::operator()(const Ubo* lhs, const Ubo* rhs) const {
    return lhs->GetId() < rhs->GetId();
  }

  UboRegistry::UboRegistry(const rx::observable<UboEvent*>& events):
    sub_on_created_(),
    sub_on_destroyed_() {
    sub_on_created_ = events
      .filter(UboCreatedEvent::Filter)
      .map(UboCreatedEvent::Cast)
      .subscribe([this](UboCreatedEvent* event) {
        const auto ubo = event->GetUbo();
        MCC_ASSERT(ubo);
        return OnCreated(ubo);
      });
    sub_on_destroyed_ = events
      .filter(UboDestroyedEvent::Filter)
      .map(UboDestroyedEvent::Cast)
      .subscribe([this](UboDestroyedEvent* event) {
        const auto ubo = event->GetUbo();
        MCC_ASSERT(ubo);
        return OnDestroyed(ubo);
      });
  }

  UboRegistry::~UboRegistry() {
    if(sub_on_created_.is_subscribed())
      sub_on_created_.unsubscribe();
    if(sub_on_destroyed_.is_subscribed())
      sub_on_destroyed_.unsubscribe();
  }

  void UboRegistry::OnCreated(const Ubo* ubo) {
    const auto result = ubos_.insert(ubo);
    LOG_IF(ERROR, !result.second) << "failed to register: " << ubo->ToString();
  }

  void UboRegistry::OnDestroyed(const Ubo* ubo) {
    const auto result = ubos_.erase(ubo);
    LOG_IF(ERROR, !result) << "failed to deregister: " << ubo->ToString();
  }
}