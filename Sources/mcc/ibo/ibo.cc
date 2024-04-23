#include "mcc/ibo/ibo.h"

#include <sstream>

namespace mcc::ibo {
  static rx::subject<IboEvent*> events_;
  static std::set<Ibo*, Ibo::IdComparator> ibos_;

  Ibo::Ibo(const IboId id):
    id_(id) {
    Publish<IboCreatedEvent>(id);
  }

  Ibo::~Ibo() {
    Publish<IboDestroyedEvent>(GetId());
  }

  Ibo* Ibo::New(const IboId id) {
    //TODO: check for duplicates
    MCC_ASSERT(IsValidIboId(id));
    return new Ibo(id);
  }

  void Ibo::Publish(IboEvent* event) {
    MCC_ASSERT(event);
    const auto& subscriber = events_.get_subscriber();
    return subscriber.on_next(event);
  }

  rx::observable<IboEvent*> OnIboEvent() {
    return events_.get_observable();
  }

  rx::observable<Ibo*> GetRegisteredIbos() {
    return rx::observable<>::iterate(ibos_);
  }

  std::string Ibo::ToString() const {
    std::stringstream ss;
    ss << "Ibo(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }
}