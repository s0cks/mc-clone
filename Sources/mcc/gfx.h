#ifndef MCC_GFX_H
#define MCC_GFX_H

#ifdef __APPLE__

#define GL_SILENCE_DEPRECATION 1
#include <OpenGL/gl3.h>

#else

#include <GL/gl3.h>

#endif//__APPLE__


#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <glog/logging.h>

namespace mcc {
  static inline std::ostream&
  operator<<(std::ostream& stream, const glm::vec3 rhs) {
    return stream << "(" << rhs[0] << ", " << rhs[1] << ", " << rhs[2] << ")";
  }

  static inline std::ostream&
  operator<<(std::ostream& stream, const glm::vec2 rhs) {
    return stream << "(" << rhs[0] << ", " << rhs[1] << ")";
  }

#ifdef MCC_DEBUG

#define CHECK_GL(Severity) ({                                     \
  const auto error = glGetError();                                \
  LOG_IF(Severity, error != GL_NO_ERROR) << "glError: " << error; \
})

#else

#endif//MCC_DEBUG

  typedef GLuint VertexArrayObjectId;
  static constexpr const VertexArrayObjectId kInvalidVertexArrayObject = 0;
  class VertexArrayObject {
  protected:
    VertexArrayObjectId id_;
  public:
    explicit VertexArrayObject(const VertexArrayObjectId id):
      id_(id) {
    }
    VertexArrayObject():
      id_(kInvalidVertexArrayObject) {
      glGenVertexArrays(1, &id_);
      CHECK_GL(FATAL);
    }
    VertexArrayObject(const VertexArrayObject& rhs):
      id_(rhs.id_) {
    }
    ~VertexArrayObject() = default;

    VertexArrayObjectId id() const {
      return id_;
    }

    void Bind() const {
      glBindVertexArray(id_);
      CHECK_GL(FATAL);
    }

    void Unbind() const {
      glBindVertexArray(kInvalidVertexArrayObject);
      CHECK_GL(FATAL);
    }

    void Delete() {
      glDeleteVertexArrays(1, &id_);
      CHECK_GL(FATAL);
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
  };

  class VertexArrayObjectBindScope {
  private:
    const VertexArrayObject& vao_;
  public:
    VertexArrayObjectBindScope(const VertexArrayObject& vao):
      vao_(vao) {
      vao_.Bind();
    }
    ~VertexArrayObjectBindScope() {
      vao_.Unbind();
    }
  };

  typedef GLuint VertexBufferObjectId;
  static constexpr VertexBufferObjectId kInvalidVertexBufferObject = 0;
  class VertexBufferObject {
  protected:
    VertexBufferObjectId id_;
  public:
    explicit VertexBufferObject(const VertexBufferObjectId id):
      id_(id) {
    }
    VertexBufferObject(const VertexBufferObject& rhs):
      id_(rhs.id_) {
    }
    virtual ~VertexBufferObject() = default;

    VertexBufferObjectId id() const {
      return id_;
    }

    void Bind() {
      glBindBuffer(GL_ARRAY_BUFFER, id_);
      CHECK_GL(FATAL);
    }

    void Unbind() {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      CHECK_GL(FATAL);
    }

    void Delete() {
      glDeleteBuffers(1, &id_);
      CHECK_GL(FATAL);
    }

    void operator=(const VertexBufferObject& rhs) {
      id_ = rhs.id_;
    }

    void operator=(const VertexBufferObjectId& rhs) {
      id_ = rhs;
    }

    explicit operator VertexBufferObjectId () const {
      return id_;
    }

    bool operator==(const VertexBufferObject& rhs) const {
      return id_ == rhs.id_;
    }

    bool operator==(const VertexBufferObjectId& rhs) const {
      return id_ == rhs;
    }

    bool operator!=(const VertexBufferObject& rhs) const {
      return id_ != rhs.id_;
    }

    bool operator!=(const VertexBufferObjectId& rhs) const {
      return id_ != rhs;
    }

    friend std::ostream& operator<<(std::ostream& stream, const VertexBufferObject& rhs) {
      stream << "VertexBufferObject(";
      stream << "id=" << rhs.id_;
      stream << ")";
      return stream;
    }
  };

  typedef GLuint IndexBufferObjectId;
  static constexpr IndexBufferObjectId kInvalidIndexBufferObject = 0;
  class IndexBufferObject {
  protected:
    IndexBufferObjectId id_;
  public:
    explicit IndexBufferObject(const IndexBufferObjectId id):
      id_(id) {
    }
    IndexBufferObject(const IndexBufferObject& rhs):
      id_(rhs.id_) {
    }
    virtual ~IndexBufferObject() = default;

    IndexBufferObjectId id() const {
      return id_;
    }

    void Bind() {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
      CHECK_GL(FATAL);
    }

    void Unbind() {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      CHECK_GL(FATAL);
    }

    void Delete() {
      glDeleteBuffers(1, &id_);
      CHECK_GL(FATAL);
    }

    void operator=(const IndexBufferObject& rhs) {
      id_ = rhs.id_;
    }

    void operator=(const IndexBufferObjectId& rhs) {
      id_ = rhs;
    }

    explicit operator IndexBufferObjectId () const {
      return id_;
    }

    bool operator==(const IndexBufferObject& rhs) const {
      return id_ == rhs.id_;
    }

    bool operator==(const IndexBufferObjectId& rhs) const {
      return id_ == rhs;
    }

    bool operator!=(const IndexBufferObject& rhs) const {
      return id_ != rhs.id_;
    }

    bool operator!=(const IndexBufferObjectId& rhs) const {
      return id_ != rhs;
    }

    friend std::ostream& operator<<(std::ostream& stream, const IndexBufferObject& rhs) {
      stream << "IndexBufferObject(";
      stream << "id=" << rhs.id_;
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_GFX_H