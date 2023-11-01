#ifndef MCC_INDEX_H
#define MCC_INDEX_H

#include <vector>
#include "mcc/gfx.h"

namespace mcc {
  typedef GLuint Index;
  typedef std::vector<Index> IndexList;

  static inline std::ostream&
  operator<<(std::ostream& stream, const IndexList& rhs) {
    stream << "[";
    for(auto idx = 0; idx < rhs.size(); idx++) {
      stream << rhs[idx];
      if((idx + 1) < rhs.size())
        stream << ", ";
    }
    stream << "]";
    return stream;
  }

  class IndexBuffer : public IndexBufferTemplate<Index, GL_UNSIGNED_INT> {
  public:
    explicit IndexBuffer(const BufferObjectId id = kInvalidBufferObject):
      IndexBufferTemplate(id) {
    }
    explicit IndexBuffer(const Index* indices, const uint64_t num_indices):
      IndexBufferTemplate(indices, num_indices) {
      Unbind();
    }
    explicit IndexBuffer(const IndexList& indices):
      IndexBuffer(&indices[0], indices.size()) {
    }
    IndexBuffer(const IndexBuffer& rhs):
      IndexBufferTemplate(rhs) {
    }
    ~IndexBuffer() override = default;

    void operator=(const IndexBuffer& rhs) {
      IndexBufferTemplate::operator=(rhs);
    }

    void operator=(const BufferObjectId& rhs) {
      BufferObject::operator=(rhs);
    }

    friend std::ostream& operator<<(std::ostream& stream, const IndexBuffer& rhs) {
      stream << "mesh::IndexBuffer(";
      stream << "id=" << rhs.id() << ", ";
      stream << "index_size=" << rhs.index_size() << ", ";
      stream << "length=" << rhs.length() << ", ";
      stream << "size=" << rhs.size();
      stream << "usage=" << rhs.usage() << ", ";
      stream << "target=" << rhs.target();
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_INDEX_H