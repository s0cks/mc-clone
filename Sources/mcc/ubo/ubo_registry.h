#ifndef MCC_UBO_REGISTRY_H
#define MCC_UBO_REGISTRY_H

#include "mcc/ubo/ubo_id.h"
#include "mcc/ubo/ubo_events.h"

namespace mcc::ubo {
  class UboRegistry {
  public:
    struct Comparator {
      bool operator()(const Ubo* lhs, const Ubo* rhs) const;
    };
  protected:
    rx::subscription sub_on_created_;
    rx::subscription sub_on_destroyed_;
    std::set<const Ubo*, Comparator> ubos_;
    void OnCreated(const Ubo* ubo);
    void OnDestroyed(const Ubo* ubo);
  public:
    explicit UboRegistry(const rx::observable<UboEvent*>& events);
    explicit UboRegistry(const rx::subject<UboEvent*>& events):
      UboRegistry(events.get_observable()) {
    }
    virtual ~UboRegistry();
    rx::observable<const Ubo*> GetAll() const {
      return rx::observable<>::iterate(ubos_);
    }
  };
}

#endif //MCC_UBO_REGISTRY_H