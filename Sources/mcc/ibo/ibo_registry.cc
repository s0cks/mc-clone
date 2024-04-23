#include "mcc/ibo/ibo_registry.h"

#include "mcc/ibo/ibo.h"

namespace mcc::ibo {
  bool IboRegistry::IdComparator::operator() (const Ibo* lhs, const Ibo* rhs) const {
    return lhs->GetId() < rhs->GetId();
  }

  template<class E>
  static inline rx::observable<E*>
  FilterEvents(const rx::observable<IboEvent*>& events) {
    return events
      .filter(E::Filter)
      .map(E::Cast);
  }

  IboRegistry::IboRegistry(const rx::observable<IboEvent*>& events):
    ibos_(),
    on_created_sub_(),
    on_destroyed_sub_() {
    on_created_sub_ = FilterEvents<IboCreatedEvent>(events)
      .subscribe([this](IboCreatedEvent* event) {
        return Register(event->GetIbo());
      });
    on_destroyed_sub_ = FilterEvents<IboDestroyedEvent>(events)
      .subscribe([this](IboDestroyedEvent* event) {
        return Deregister(event->GetIbo());
      });
  }

  IboRegistry::~IboRegistry() {
    on_created_sub_.unsubscribe();
    on_destroyed_sub_.unsubscribe();
  }

  void IboRegistry::Register(const Ibo* ibo) {
    const auto result = ibos_.insert(ibo);
    LOG_IF(WARNING, !result.second) << "failed to register: " << ibo->ToString();
  }

  void IboRegistry::Deregister(const Ibo* ibo) {
    const auto result = ibos_.erase(ibo);
    LOG_IF(WARNING, result != 1) << "failed to deregister: " << ibo->ToString();
  }

  rx::observable<const UByteIbo*> IboRegistry::GetAllUByteIbos() const {
    return GetAllIbos()
      .filter(UByteIbo::Filter)
      .map(UByteIbo::Cast);
  }

  rx::observable<const UShortIbo*> IboRegistry::GetAllUShortIbos() const {
    return GetAllIbos()
      .filter(UShortIbo::Filter)
      .map(UShortIbo::Cast);
  }

  rx::observable<const UIntIbo*> IboRegistry::GetAllUIntIbos() const {
    return GetAllIbos()
      .filter(UIntIbo::Filter)
      .map(UIntIbo::Cast);
  }
}