#include "mcc/ibo/ibo.h"

#include <sstream>
#include "mcc/ibo/ibo_factory.h"

namespace mcc::ibo {
  static rx::subject<IboEvent*> events_;
  static std::set<Ibo*, Ibo::IdComparator> ibos_;

  void Ibo::Publish(IboEvent* event) {
    MCC_ASSERT(event);
    const auto& subscriber = events_.get_subscriber();
    return subscriber.on_next(event);
  }

  void Ibo::BindIbo(const IboId id) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    CHECK_GL(FATAL);
  }

  rx::observable<IboEvent*> OnIboEvent() {
    return events_.get_observable();
  }

  rx::observable<Ibo*> GetRegisteredIbos() {
    return rx::observable<>::iterate(ibos_);
  }

  void Ibo::Destroy() {
    NOT_IMPLEMENTED(FATAL); //TODO: implement
  }

  void Ibo::Update(const uint64_t offset, const uint8_t* data, const uint64_t len) {
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(len), (const GLvoid*) data);
    CHECK_GL(FATAL);
  }

  void Ibo::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetId());
    CHECK_GL(FATAL);
  }

  void Ibo::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, kDefaultIboId);
    CHECK_GL(FATAL);
  }

  std::string UByteIbo::ToString() const {
    std::stringstream ss;
    ss << "UByteIbo(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }

  std::string UShortIbo::ToString() const {
    std::stringstream ss;
    ss << "UShortIbo(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }

  std::string UIntIbo::ToString() const {
    std::stringstream ss;
    ss << "UIntIbo(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }

  UByteIbo* UByteIbo::New(const Index* indices, const uint64_t num_indices, const Usage usage) {
    IboFactory<UnsignedByte> factory(indices, num_indices);
    return factory.Create(usage)
      .map([usage,num_indices](const IboId id) {
        return UByteIbo::New(id, usage, num_indices);
      })
      .as_blocking()
      .first();
  }

  UShortIbo* UShortIbo::New(const Index* indices, const uint64_t num_indices, const Usage usage) {
    IboFactory<UnsignedShort> factory(indices, num_indices);
    return factory.Create(usage)
      .map([usage,num_indices](const IboId id) {
        return UShortIbo::New(id, usage, num_indices);
      })
      .as_blocking()
      .first();
  }

  UIntIbo* UIntIbo::New(const UIntIbo::Index* indices, const uint64_t num_indices, const Usage usage) {
    IboFactory<UnsignedInt> factory(indices, num_indices);
    return factory.Create(usage)
      .map([usage,num_indices](const IboId id) {
        return UIntIbo::New(id, usage, num_indices);
      })
      .as_blocking()
      .first();
  }
}