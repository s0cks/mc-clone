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

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <glog/logging.h>

namespace mcc {
#ifdef MCC_DEBUG

#define CHECK_GL(Severity) ({                                     \
  const auto error = glGetError();                                \
  LOG_IF(Severity, error != GL_NO_ERROR) << "glError: " << error; \
})

#else

#endif//MCC_DEBUG

  enum GlObjectUsage {
    kDynamicUsage = GL_DYNAMIC_DRAW,
    kStaticUsage = GL_STATIC_DRAW,
    kStreamUsage = GL_STREAM_DRAW,
    kDefaultUsage = kDynamicUsage,
  };

  static inline std::ostream&
  operator<<(std::ostream& stream, const GlObjectUsage& rhs) {
    switch(rhs) {
      case kDynamicUsage:
        return stream << "Dynamic";
      case kStaticUsage:
        return stream << "Static";
      default:
        return stream << "<unknown GlObjectUsage: " << rhs << ">";
    }
  }

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

  typedef GLuint BufferObjectId;
  static constexpr const BufferObjectId kInvalidBufferObject = 0;

  enum BufferObjectTarget {
    kVertex = GL_ARRAY_BUFFER,
    kIndex = GL_ELEMENT_ARRAY_BUFFER,
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

  class BufferObject {
  protected:
    BufferObjectId id_;

    BufferObject():
      id_(kInvalidBufferObject) {
      glGenBuffers(1, &id_);
      CHECK_GL(FATAL);
    }
  public:
    explicit BufferObject(const BufferObjectId id):
      id_(id) {
    }
    BufferObject(const BufferObject& rhs):
      id_(rhs.id_) {
    }
    virtual ~BufferObject() = default;

    BufferObjectId id() const {
      return id_;
    }

    virtual GlObjectUsage usage() const = 0;
    virtual BufferObjectTarget target() const = 0;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    void Delete() {
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

  class VertexBufferObject : public BufferObjectTemplate<kVertex> {
  public:
    explicit VertexBufferObject(const BufferObjectId id):
      BufferObjectTemplate(id) {  
    }
    VertexBufferObject() = default;
    VertexBufferObject(const VertexBufferObject& rhs):
      BufferObjectTemplate(rhs) {
    }
    ~VertexBufferObject() override = default;
    virtual uint64_t length() const = 0;
    virtual uint64_t vertex_size() const = 0;
    
    virtual uint64_t size() const {
      return length() * vertex_size();
    }
  };

  template<typename Vertex, const GlObjectUsage Usage = kDefaultUsage>
  class VertexBufferTemplate : public VertexBufferObject {
  public:
    static const uint64_t kVertexSize = sizeof(Vertex);
    typedef std::vector<Vertex> VertexList;
  protected:
    uint64_t length_;

    explicit VertexBufferTemplate(const BufferObjectId id):
      VertexBufferObject(id) {
    }
    VertexBufferTemplate():
      VertexBufferObject(),
      length_(0) {
    }
    explicit VertexBufferTemplate(const Vertex* vertices, const uint64_t num_vertices):
      VertexBufferObject(),
      length_(num_vertices) {
      BindBufferData(vertices, num_vertices);
    }
  public:
    ~VertexBufferTemplate() override = default;

    void BufferData(const Vertex* vertices, const uint64_t num_vertices) {
      DLOG_IF(ERROR, num_vertices == 0) << "binding VertexBufferObject w/ 0 vertices."; 
      glBufferData(target(), num_vertices * kVertexSize, vertices, Usage);
      CHECK_GL(FATAL);
    }

    void BindBufferData(const Vertex* vertices, const uint64_t num_vertices) {
      Bind();
      BufferData(vertices, num_vertices);
    }

    void BufferData(const uint64_t size) {
      DLOG_IF(ERROR, size == 0) << "binding VertexBufferObject w/ size of " << size; 
      glBufferData(target(), size, NULL, Usage);
      CHECK_GL(FATAL);
    }

    void BindBufferData(const uint64_t size) {
      Bind();
      BufferData(size);
    }

    GlObjectUsage usage() const override {
      return Usage;
    }

    uint64_t length() const override {
      return length_;
    }

    uint64_t vertex_size() const override {
      return kVertexSize;
    }
  };

  class IndexBufferObject : public BufferObjectTemplate<kIndex> {
  public:
    explicit IndexBufferObject(const BufferObjectId id):
      BufferObjectTemplate(id) {
    }
    IndexBufferObject() = default;
    IndexBufferObject(const IndexBufferObject& rhs):
      BufferObjectTemplate(rhs) {  
    }
    ~IndexBufferObject() override = default;
    virtual uint64_t length() const = 0;
    virtual uint64_t index_size() const = 0;

    virtual uint64_t size() const {
      return length() * index_size();
    }
  };

  template<typename Index, const GLenum Type, const GlObjectUsage Usage = kDefaultUsage>
  class IndexBufferTemplate : public IndexBufferObject {
  public:
    static const uint64_t kIndexSize = sizeof(Index);
    typedef std::vector<Index> IndexList;
  protected:
    uint64_t length_;

    explicit IndexBufferTemplate(const BufferObjectId id):
      IndexBufferObject(id) {
    }
    IndexBufferTemplate() = default;
    explicit IndexBufferTemplate(const Index* indices,
                                 const uint64_t num_indices):
      IndexBufferObject(),
      length_(num_indices) {
      BindBufferData(indices, num_indices);
    }
  public:
    ~IndexBufferTemplate() override = default;

    void BufferData(const Index* indices, const uint64_t num_indices) {
      DLOG_IF(ERROR, num_indices == 0) << "creating IndexBufferObject w/ 0 indices.";
      glBufferData(target(), num_indices * kIndexSize, &indices[0], Usage);
      CHECK_GL(FATAL);
    }

    void BindBufferData(const Index* indices, const uint64_t num_indices) {
      Bind();
      BufferData(indices, num_indices);
    }

    void BufferData(const uint64_t size) {
      DLOG_IF(ERROR, size == 0) << "creating IndexBufferObject w/ size " << size;
      glBufferData(target(), size, NULL, Usage);
      CHECK_GL(FATAL);
    }

    void BindBufferData(const uint64_t size) {
      Bind();
      BufferData(size);
    }

    GlObjectUsage usage() const override {
      return Usage;
    }

    uint64_t length() const override {
      return length_;
    }

    uint64_t index_size() const override {
      return kIndexSize;
    }

    GLenum type() const {
      return Type;
    }

    void operator=(const IndexBufferTemplate<Index, Usage>& rhs) {
      BufferObject::operator=((const BufferObject&) rhs);
      length_ = rhs.length_;
    }
  };
}

#ifdef M_PI
#define PI M_PI
#endif //M_PI

#ifndef PI
#define PI 3.141592654
#endif //PI

#endif //MCC_GFX_H