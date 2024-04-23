#include "mcc/ibo/ibo_factory.h"

namespace mcc::ibo {
  void IboFactoryBase::BindBuffer(const IboId id) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    CHECK_GL(FATAL);
  }

  void IboFactoryBase::PutBufferData(const IboId id, const uint8_t* data, const uint64_t size, const Usage usage) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
    CHECK_GL(FATAL);
  }
}