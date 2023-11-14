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

#include "mcc/common.h"

namespace mcc {
#ifdef MCC_DEBUG

#define CHECK_GL(Severity) ({                                     \
  const auto error = glGetError();                                \
  LOG_IF(Severity, error != GL_NO_ERROR) << "glError: " << error; \
})

#else

#define CHECK_GL(Severity)

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

  namespace gfx {
    class Resource {
    public:
      Resource() = default;
      virtual ~Resource() = default;
      virtual void Bind() const = 0;
      virtual void Unbind() const = 0;
      virtual void Delete() = 0;
    };

    template<class R>
    class BindScope {
      DEFINE_NON_COPYABLE_TYPE(BindScope);
    private:
      const R& resource_;
    public:
      BindScope() = delete;
      explicit BindScope(const R& resource):
        resource_(resource) {
        resource_.Bind();
      }
      ~BindScope() {
        resource_.Unbind();
      }
    };
  }

#define DEFINE_RESOURCE_SCOPE(Resource) \
  typedef gfx::BindScope<Resource> Resource##Scope;

  typedef GLuint VertexArrayObjectId;
  static constexpr const VertexArrayObjectId kInvalidVertexArrayObject = 0;
  class VertexArrayObject : public gfx::Resource {
  protected:
    VertexArrayObjectId id_;
  public:
    explicit VertexArrayObject(const VertexArrayObjectId id):
      gfx::Resource(),
      id_(id) {
    }
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
    ~VertexArrayObject() override = default;

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

    void Delete() override {
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
  public:
    static inline void
    GenerateBatch(const uint64_t num_vaos, VertexArrayObject** vaos) {
      MCC_ASSERT((*vaos) == nullptr);
      DLOG(INFO) << "generating " << num_vaos << " VertexArrayObjects....";
      VertexArrayObjectId ids[num_vaos];
      glGenVertexArrays(num_vaos, ids);
      CHECK_GL(FATAL);
      (*vaos) = (VertexArrayObject*)malloc(sizeof(VertexArrayObject) * num_vaos);
      for(auto idx = 0; idx < num_vaos; idx++) {
        const auto vao = VertexArrayObject(ids[idx]);
        memcpy((void*) &vaos[idx], (void*) &vao, sizeof(VertexArrayObject));
      }
      DLOG(INFO) << "done.";
    }
  };
  DEFINE_RESOURCE_SCOPE(VertexArrayObject);

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

  class UniformBufferObject : public BufferObjectTemplate<kUniform> {
  public:
    explicit UniformBufferObject() = default;
    UniformBufferObject(const BufferObjectId id):
      BufferObjectTemplate(id) {
    }
    UniformBufferObject(const UniformBufferObject& rhs) = default;
    ~UniformBufferObject() override = default;
    virtual uint64_t length() const = 0;
    virtual uint64_t elem_size() const = 0;

    virtual uint64_t size() const {
      return length() * elem_size();
    }
  };

  template<typename T, const GlObjectUsage Usage = kDefaultUsage>
  class UniformBufferObjectTemplate : public UniformBufferObject {
  public:
    static constexpr const uint64_t kElementSize = sizeof(T);
  protected:
    uint64_t length_;

    UniformBufferObjectTemplate() = default;
    explicit UniformBufferObjectTemplate(const BufferObjectId id):
      UniformBufferObject(id) {
    }
    UniformBufferObjectTemplate(T* values, const uint64_t num_values):
      UniformBufferObject(),
      length_(num_values) {
      BindBufferData(values, num_values);
    }
    explicit UniformBufferObjectTemplate(const uint64_t num_values):
      UniformBufferObject(),
      length_(num_values) {
      BindBufferData(num_values);
    }
  public:
    ~UniformBufferObjectTemplate() override = default;

    void BufferData(const T* values, const uint64_t num_values) {
      DLOG_IF(ERROR, num_values == 0) << "glBufferData called w/ 0 values.";
      glBufferData(target(), num_values * kElementSize, &values[0], Usage);
      CHECK_GL(FATAL);
    }

    void BindBufferData(const T* values, const uint64_t num_values) {
      Bind();
      BufferData(values, num_values);
    }

    void BufferData(const uint64_t size) {
      DLOG_IF(ERROR, size <= 0) << "glBufferData called w/ " << size << " bytes.";
      glBufferData(target(), size * kElementSize, NULL, Usage);
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

    uint64_t elem_size() const override {
      return kElementSize;
    }

    void operator=(const UniformBufferObjectTemplate<T, Usage>& rhs) {
      BufferObject::operator=((const BufferObject&) rhs);
      length_ = rhs.length_;
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