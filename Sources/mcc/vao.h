#ifndef MCC_VAO_H
#define MCC_VAO_H

#include "mcc/gfx.h"

namespace mcc {
  class VertexArrayObject;
  typedef std::shared_ptr<VertexArrayObject> Vao;

  typedef GLuint VertexArrayObjectId;
  static constexpr const VertexArrayObjectId kInvalidVertexArrayObject = 0;
  class VertexArrayObject : public gfx::Resource {
  protected:
    VertexArrayObjectId id_;

    VertexArrayObject():
      gfx::Resource(),
      id_(kInvalidVertexArrayObject) {
      glGenVertexArrays(1, &id_);
      CHECK_GL(FATAL);
    }
    VertexArrayObject(const VertexArrayObject& rhs):
      gfx::Resource(),
      id_(rhs.id_) {
    }
  public:
    ~VertexArrayObject() override {
      if(id_ == kInvalidVertexArrayObject)
        return;

      glDeleteVertexArrays(1, &id_);
      CHECK_GL(FATAL);
    }

    VertexArrayObjectId id() const {
      return id_;
    }

    void Bind() const override {
      glBindVertexArray(id_);
      CHECK_GL(FATAL);
    }

    void Unbind() const override {
      glBindVertexArray(kInvalidVertexArrayObject);
      CHECK_GL(FATAL);
    }

    bool IsValid() const {
      return id() != kInvalidVertexArrayObject;
    }

    void operator=(const VertexArrayObject& rhs) {
      id_ = rhs.id_;
    }

    void operator=(const VertexArrayObjectId& rhs) {
      id_ = rhs;
    }

    explicit operator VertexArrayObjectId () const {
      return id_;
    }

    bool operator==(const VertexArrayObject& rhs) const {
      return id_ == rhs.id_;
    }

    bool operator==(const VertexArrayObjectId& rhs) const {
      return id_ == rhs;
    }

    bool operator!=(const VertexArrayObject& rhs) const {
      return id_ != rhs.id_;
    }

    bool operator!=(const VertexArrayObjectId& rhs) const {
      return id_ != rhs;
    }

    friend std::ostream& operator<<(std::ostream& stream, const VertexArrayObject& rhs) {
      stream << "VertexArrayObject(";
      stream << "id=" << rhs.id_;
      stream << ")";
      return stream;
    }
  public:
    static inline Vao
    New() {
      struct VertexArrayObjectEnabler : public VertexArrayObject{};
      return std::make_shared<VertexArrayObjectEnabler>();
    }
  };
  
  class VertexArrayObjectScope {
  protected:
    Vao vao_;
  public:
    explicit VertexArrayObjectScope(const Vao& vao):
      vao_(vao) {
      vao_->Bind();  
    }
    ~VertexArrayObjectScope() {
      vao_->Unbind();
    }
  };
}

#endif //MCC_VAO_H