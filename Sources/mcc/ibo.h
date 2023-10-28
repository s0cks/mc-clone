#ifndef MCC_IBO_H
#define MCC_IBO_H

#include "mcc/gfx.h"

namespace mcc {
  typedef GLuint IboId;

  static constexpr const IboId kInvalidIbo = 0;

  class Ibo {
  private:
    IboId id_;
  public:
    explicit Ibo(const IboId id = kInvalidIbo):
      id_(id) {
    }
    Ibo(const GLubyte* indices, GLsizeiptr size);
    Ibo(const std::vector<GLubyte>& indices):
      Ibo((const GLubyte*) &indices[0], indices.size()) {
    }
    Ibo(const Ibo& rhs):
      id_(rhs.id_) {
    }
    ~Ibo() = default;

    void Bind();
    void Unbind();
    void Delete();

    void operator=(const Ibo& rhs) {
      id_ = rhs.id_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Ibo& rhs) {
      stream << "Ibo(";
      stream << "id=" << rhs.id_;
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_IBO_H