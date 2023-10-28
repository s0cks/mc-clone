#ifndef MCC_VAO_H
#define MCC_VAO_H

#include "mcc/gfx.h"
#include "mcc/vbo.h"

namespace mcc {
  typedef GLuint VaoId;

  static constexpr const VaoId kInvalidVaoId = 0;

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
    void Bind();
    void Unbind();
    void Delete();

    void operator=(const Vao& rhs) {
      id_ = rhs.id_;
    }
  };

  class VaoScope {
  private:
    Vao& vao_;
  public:
    VaoScope() = delete;
    VaoScope(Vao& vao):
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