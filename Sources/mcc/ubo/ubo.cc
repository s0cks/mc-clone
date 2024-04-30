#include "mcc/ubo/ubo.h"
#include <sstream>

namespace mcc::ubo {
  static rx::subject<UboEvent*> events_;
  static UboRegistry registry_(events_);

  const UboRegistry& GetRegistry() {
    return registry_;
  }

  rx::observable<UboEvent*> OnUboEvent() {
    return events_.get_observable();
  }

  void Ubo::Publish(UboEvent* event) {
    MCC_ASSERT(event);
    const auto& subscriber = events_.get_subscriber();
    return subscriber.on_next(event);
  }

  void Ubo::BindUbo(const UboId id) {
    MCC_ASSERT(IsValidUboId(id));
    glBindBuffer(Ubo::kGlTarget, id);
    CHECK_GL(FATAL);
  }

  void Ubo::DeleteUbos(const UboId* ids, const uint64_t num_ids) {
    MCC_ASSERT(num_ids > 0);
    glDeleteBuffers(num_ids, ids);
    CHECK_GL(FATAL);
  }

  void Ubo::InitUbo(const uint8_t* bytes, const uint64_t num_bytes, const GLenum usage) {
    MCC_ASSERT(num_bytes > 0);
    glBufferData(Ubo::kGlTarget, num_bytes, bytes, usage);
    CHECK_GL(FATAL);
  }

  void Ubo::UpdateUbo(const uint64_t offset, const uint8_t* bytes, const uint64_t num_bytes) {
    MCC_ASSERT(offset >= 0);
    MCC_ASSERT(num_bytes > 0);
    glBufferSubData(Ubo::kGlTarget, offset, num_bytes, bytes);
    CHECK_GL(FATAL);
  }

  Ubo::Ubo(const UboId id,
           const uint64_t elem_size,
           const uint64_t len,
           const GLenum usage):
    id_(id),
    elem_size_(elem_size),
    length_(len),
    usage_(usage) {
    Publish<UboCreatedEvent>(this);
  }

  Ubo::~Ubo() {
    Publish<UboDestroyedEvent>(this);
  }

  std::string Ubo::ToString() const {
    std::stringstream ss;
    ss << "Ubo(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }
}