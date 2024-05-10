#include "mcc/vao/vao.h"

#include <set>
#include <sstream>

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
    ObjectTemplate(id) {
    vaos_.insert(this);
    Publish<VaoCreatedEvent>(this);
  }

  Vao::~Vao() {
    Publish<VaoDestroyedEvent>(this);
  }

  void Vao::BindVao(const VaoId id) {
    MCC_ASSERT(IsValidVaoId(id) || id == 0);
    glBindVertexArray(id);
    CHECK_GL(FATAL);
  }

  void Vao::DeleteVaos(const VaoId* ids, const uint64_t num_ids) {
    MCC_ASSERT(ids != nullptr);
    MCC_ASSERT(num_ids >= 1);
    glDeleteVertexArrays(num_ids, ids);
    CHECK_GL(FATAL);
  }

  void Vao::Publish(VaoEvent* event) {
    MCC_ASSERT(event);
    const auto& subscriber = events_.get_subscriber();
    subscriber.on_next(event);
  }

  std::string Vao::ToString() const {
    std::stringstream ss;
    ss << "Vao(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }

  Vao* Vao::New(const VaoId id) {
    //TODO: check vaos_
    MCC_ASSERT(IsValidVaoId(id));
    return new Vao(id);
  }
}