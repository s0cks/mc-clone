#ifndef MCC_VAO_H
#define MCC_VAO_H

#include <iostream>
#include "mcc/gfx.h"
#include "mcc/vbo.h"

namespace mcc {
  typedef GLuint VaoId;

  static constexpr const VaoId kInvalidVao = 0;

  class Vao {
  public:
    VaoId id_;
  public:
    Vao();
    explicit Vao(const VaoId id):
      id_(id) {
    }
    Vao(const Vao& rhs):
      id_(rhs.id_) {
    }
    ~Vao() = default;

    void Link(Vbo& vbo, GLuint layout);
    void Bind() const;
    void Unbind() const;
    void Delete();

    Vao& operator=(const Vao& rhs) {
      id_ = rhs.id_;
      return *this;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Vao& rhs) {
      stream << "Vao(";
      stream << "id=" << rhs.id_;
      stream << ")";
      return stream;
    }
  };

  class VaoScope {
  private:
    const Vao& vao_;
  public:
    VaoScope() = delete;
    explicit VaoScope(const Vao& vao):
      vao_(vao) {
      vao_.Bind();
    }
    VaoScope(const VaoScope& rhs) = delete;
    ~VaoScope() {
      vao_.Unbind();
    }

    void operator=(const VaoScope& rhs) = delete;
  };
}

#endif //MCC_VAO_H