#include "mcc/ibo/ibo.h"

#include <sstream>
#include "mcc/ibo/ibo_builder.h"
#include "mcc/ibo/ibo_registry.h"

namespace mcc::ibo {
  static rx::subject<IboEvent*> events_;
  static IboRegistry registry_(events_.get_observable());

  void Ibo::PublishEvent(IboEvent* event) {
    MCC_ASSERT(event);
    const auto& subscriber = events_.get_subscriber();
    return subscriber.on_next(event);
  }

  void Ibo::BindIbo(const IboId id) {
    glBindBuffer(kGlTarget, id);
    CHECK_GL(FATAL);
  }

  void Ibo::InitBufferData(const uint8_t* bytes, const uint64_t num_bytes, const Usage usage) {
    glBufferData(kGlTarget, num_bytes, bytes, usage);
    CHECK_GL(FATAL);
  }

  void Ibo::UpdateBufferData(const uint64_t offset, const uint8_t* bytes, const uint64_t num_bytes) {
    glBufferSubData(kGlTarget, offset, num_bytes, bytes);
    CHECK_GL(FATAL);
  }

  rx::observable<IboEvent*> OnIboEvent() {
    return events_.get_observable();
  }

  const IboRegistry& GetRegistry() {
    return registry_;
  }

  void Ibo::Destroy() {
    NOT_IMPLEMENTED(FATAL); //TODO: implement
  }

  void Ibo::Update(const uint64_t offset, const uint8_t* data, const uint64_t len) {
    glBufferSubData(kGlTarget, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(len), (const GLvoid*) data);
    CHECK_GL(FATAL);
  }

  void Ibo::Bind() const {
    glBindBuffer(kGlTarget, GetId());
    CHECK_GL(FATAL);
  }

  void Ibo::Unbind() const {
    glBindBuffer(kGlTarget, kInvalidIboId);
    CHECK_GL(FATAL);
  }

  std::string UByteIbo::ToString() const {
    using namespace units;
    std::stringstream ss;
    ss << "UByteIbo(";
    ss << "id=" << GetId();
    ss << "length=" << GetLength() << ", ";
    ss << "size=" << data::byte_t(GetSize());
    ss << ")";
    return ss.str();
  }

  std::string UShortIbo::ToString() const {
    using namespace units;
    std::stringstream ss;
    ss << "UShortIbo(";
    ss << "id=" << GetId();
    ss << "length=" << GetLength() << ", ";
    ss << "size=" << data::byte_t(GetSize());
    ss << ")";
    return ss.str();
  }

  std::string UIntIbo::ToString() const {
    using namespace units;
    std::stringstream ss;
    ss << "UIntIbo(";
    ss << "id=" << GetId() << ", ";
    ss << "length=" << GetLength() << ", ";
    ss << "size=" << data::byte_t(GetSize());
    ss << ")";
    return ss.str();
  }

  UByteIbo* UByteIbo::New(const IndexList& indices, const Usage usage) {
    UByteIboBuilder builder;
    builder.SetUsage(usage);
    builder.Append(indices);
    return builder.Build()
      .as_blocking()
      .first();
  }

  UByteIbo* UByteIbo::New(const uint64_t num_indices, const Usage usage) {
    UByteIboBuilder builder(num_indices);
    builder.SetUsage(usage);
    return builder.Build()
      .as_blocking()
      .first();
  }

  UShortIbo* UShortIbo::New(const IndexList& indices, const Usage usage) {
    UShortIboBuilder builder;
    builder.SetUsage(usage);
    builder.Append(indices);
    return builder.Build()
      .as_blocking()
      .first();
  }

  UShortIbo* UShortIbo::New(const uint64_t num_indices, const Usage usage) {
    UShortIboBuilder builder(num_indices);
    builder.SetUsage(usage);
    return builder.Build()
      .as_blocking()
      .first();
  }

  UIntIbo* UIntIbo::New(const uint64_t num_indices, const Usage usage) {
    UIntIboBuilder builder(num_indices);
    builder.SetUsage(usage);
    return builder.Build()
      .as_blocking()
      .first();
  }

  UIntIbo* UIntIbo::New(const IndexList& indices, const Usage usage) {
    UIntIboBuilder builder;
    builder.SetUsage(usage);
    builder.Append(indices);
    return builder.Build()
      .as_blocking()
      .first();
  }
}