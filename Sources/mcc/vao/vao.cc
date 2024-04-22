#include "mcc/vao/vao.h"

#include <set>

namespace mcc::vao {
  static rx::subject<VaoEvent*> events_;
  static std::set<Vao*, Vao::VaoIdComparator> vaos_;

  rx::observable<VaoEvent*> OnVaoEvent() {
    return events_.get_observable();
  }
  
  rx::observable<Vao*> GetRegisteredVaos() {
    return rx::observable<>::iterate(vaos_);
  }

  Vao::Vao(const VaoId id):
    id_(id) {
    vaos_.insert(this);
    Publish<VaoCreatedEvent>(id);
  }

  Vao::~Vao() {

  }

  void Vao::Publish(VaoEvent* event) {
    MCC_ASSERT(event);
    const auto& subscriber = events_.get_subscriber();
    subscriber.on_next(event);
  }

  Vao* Vao::New(const VaoId id) {
    //TODO: check vaos_
    MCC_ASSERT(IsValidVaoId(id));
    return new Vao(id);
  }
}