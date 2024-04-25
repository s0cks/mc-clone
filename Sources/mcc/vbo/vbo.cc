#include "mcc/vbo/vbo.h"

#include <units.h>

namespace mcc::vbo {
  static rx::subject<VboEvent*> events_;

  rx::observable<VboEvent*> OnVboEvent() {
    return events_.get_observable();
  }

  rx::observable<VboEvent*> OnVboEvent(const VboId id) {
    return OnVboEvent()
      .filter([id](VboEvent* event) {
        return event
            && event->GetVboId() == id;
      });
  }

  void Vbo::BindVbo(const VboId id) {
    MCC_ASSERT(IsValidVboId(id));
    glBindBuffer(GL_ARRAY_BUFFER, id);
    CHECK_GL(FATAL);
  }

  void Vbo::DeleteVbos(const VboId* ids, const int num_ids) {
    //TODO: check ids?
    glDeleteBuffers(num_ids, ids);
    CHECK_GL(FATAL);
  }

  void Vbo::PutVboData(const uint8_t* bytes, const uint64_t num_bytes, const VboUsage usage) {
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) num_bytes, bytes, usage);
    CHECK_GL(FATAL);
  }

  void Vbo::UpdateVboData(const uint64_t offset, const uint8_t* bytes, const uint64_t num_bytes) {
    glBufferSubData(GL_ARRAY_BUFFER, (GLintptr) offset, (GLsizeiptr) num_bytes, (const GLvoid*) bytes);
    CHECK_GL(FATAL);
  }

  void Vbo::PublishEvent(VboEvent* event) {
    MCC_ASSERT(event);
    const auto& subscriber = events_.get_subscriber();
    subscriber.on_next(event);
  }

  std::string Vbo::ToString() const {
    using namespace units;
    std::stringstream ss;
    ss << "Vbo(";
    ss << "id=" << GetId();
    ss << "length=" << GetLength() << ", ";
    ss << "vertex_size=" << data::byte_t(GetVertexSize()) << ", ";
    ss << "size=" << data::byte_t(GetSize());
    ss << ")";
    return ss.str();
  }

  Vbo* Vbo::New(const VboId id, const uint64_t length, const uint64_t vertex_size, const VboUsage usage) {
    MCC_ASSERT(IsValidVboId(id));
    MCC_ASSERT(length > 0);
    MCC_ASSERT(vertex_size > 0);
    const auto vbo = new Vbo(id, length, vertex_size, usage);
    PublishEvent<VboCreatedEvent>(vbo);
    return vbo;
  }
}