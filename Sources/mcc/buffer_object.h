#ifndef MCC_BUFFER_OBJECT_H
#define MCC_BUFFER_OBJECT_H

#include "mcc/gfx.h"

namespace mcc {
  typedef GLuint BufferObjectId;
  static constexpr const BufferObjectId kInvalidBufferObject = 0;

  enum BufferObjectTarget {
    kVertex = GL_ARRAY_BUFFER,
    kIndex = GL_ELEMENT_ARRAY_BUFFER,
    kUniform = GL_UNIFORM_BUFFER,
  };

  static inline std::ostream&
  operator<<(std::ostream& stream, const BufferObjectTarget& rhs) {
    switch(rhs) {
      case kVertex:
        return stream << "Vertex";
      case kIndex:
        return stream << "Index";
      default:
        return stream << "<unknown BufferObjectTarget: " << rhs << ">";
    }
  }

  class BufferObject : public gfx::Resource {
  protected:
    BufferObjectId id_;

    BufferObject():
      id_(kInvalidBufferObject) {
      glGenBuffers(1, &id_);
      CHECK_GL(FATAL);
    }
  public:
    explicit BufferObject(const BufferObjectId id):
      Resource(),
      id_(id) {
    }
    BufferObject(const BufferObject& rhs):
      Resource(),
      id_(rhs.id_) {
    }
    ~BufferObject() override = default;

    BufferObjectId id() const {
      return id_;
    }

    virtual GlObjectUsage usage() const = 0;
    virtual BufferObjectTarget target() const = 0;
    void Bind() const override = 0;
    void Unbind() const override = 0;

    void Delete() override {
      glDeleteBuffers(1, &id_);
      CHECK_GL(FATAL);
    }

    void operator=(const BufferObject& rhs) {
      id_ = rhs.id_;
    }

    void operator=(const BufferObjectId& rhs) {
      id_ = rhs;
    }

    explicit operator BufferObjectId () const {
      return id_;
    }

    bool operator==(const BufferObject& rhs) const {
      return id_ == rhs.id_;
    }

    bool operator==(const BufferObjectId& rhs) const {
      return id_ == rhs;
    }

    bool operator!=(const BufferObject& rhs) const {
      return id_ != rhs.id_;
    }

    bool operator!=(const BufferObjectId& rhs) const {
      return id_ != rhs;
    }

    friend std::ostream& operator<<(std::ostream& stream, const BufferObject& rhs) {
      stream << "BufferObject(";
      stream << "id=" << rhs.id_;
      stream << ")";
      return stream;
    }
  };

  template<const BufferObjectTarget Target>
  class BufferObjectTemplate : public BufferObject {
  public:
    explicit BufferObjectTemplate(const BufferObjectId id):
      BufferObject(id) {
    }
    BufferObjectTemplate():
      BufferObject() {
      glBindBuffer(Target, id_);
    }
    BufferObjectTemplate(const BufferObjectTemplate& rhs):
      BufferObject(rhs) {
    }
    ~BufferObjectTemplate() override = default;

    BufferObjectTarget target() const override {
      return Target;
    }

    void Bind() const override {
      glBindBuffer(Target, id_);
      CHECK_GL(FATAL);
    }

    void Unbind() const override {
      glBindBuffer(Target, 0);
      CHECK_GL(FATAL);
    }
  };
}

#endif //MCC_BUFFER_OBJECT_H