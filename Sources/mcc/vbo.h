#ifndef MCC_VBO_H
#define MCC_VBO_H

#include "mcc/gfx.h"

namespace mcc {
  typedef GLuint VboId;

  static constexpr const VboId kInvalidVboId = 0;

  class Vbo {
  private:
    VboId id_;
  public:
    explicit Vbo(const VboId id = kInvalidVboId):
      id_(id) {
    }
    Vbo(const GLfloat* vertices, GLsizeiptr size);
    Vbo(const std::vector<glm::vec3>& vertices):
      Vbo((const GLfloat*) &vertices[0], vertices.size() * sizeof(float) * 3) {
    }
    Vbo(const Vbo& rhs):
      id_(rhs.id_) {
    }
    ~Vbo() = default;

    void Bind();
    void Unbind();
    void Delete();

    void operator=(const Vbo& rhs) {
      id_ = rhs.id_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Vbo& rhs) {
      stream << "Vbo(";
      stream << "id=" << rhs.id_;
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_VBO_H