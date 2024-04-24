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

  void Vbo::PutVboData(const uint8_t* bytes, const uint64_t num_bytes, const Usage usage) {
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) num_bytes, bytes, usage);
    CHECK_GL(FATAL);
  }

  void Vbo::UpdateVboData(const uint64_t offset, const uint8_t* bytes, const uint64_t num_bytes) {
    glBufferSubData(GL_ARRAY_BUFFER, (GLintptr) offset, (GLsizeiptr) num_bytes, (const GLvoid*) bytes);
    CHECK_GL(FATAL);
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
}