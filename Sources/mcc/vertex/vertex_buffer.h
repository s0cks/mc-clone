#ifndef MCC_VERTEX_BUFFER_H
#define MCC_VERTEX_BUFFER_H

#include "mcc/buffer_object.h"

namespace mcc {
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

  template<typename... EnabledAttributes>
  class VertexBufferObjectDrawScope {
  private:
    const VertexBufferObject& vbo_;

    template<typename A>
    inline void Enable() { }

    template<typename A, typename B, typename... Tail>
    inline void Enable() {
      B::Enable();
      Enable<A, Tail...>();
    }

    template<typename... Attrs>
    inline void EnableAll() {
      Enable<void, Attrs...>();
    }
  public:
    VertexBufferObjectDrawScope(const VertexBufferObject& vbo):
      vbo_(vbo) {
      vbo_.Bind();
      EnableAll<EnabledAttributes...>();
    }
    ~VertexBufferObjectDrawScope() {
      vbo_.Unbind();
    }

    void Draw(const GLenum mode, GLint first, GLsizei count) const {
      glDrawArrays(mode, first, count);
      CHECK_GL(FATAL);
    }

    inline void Draw(const GLenum mode, GLint first = 0) const { 
      return Draw(mode, first, vbo_.length());
    }
  };

  template<const GLuint Index, 
           const GLint Size,
           const GLenum Type,
           const GLboolean Normalized,
           const GLsizei Stride,
           const uword Offset>
  class VertexBufferAttribute {
  public:
    static inline GLuint GetIndex() {
      return Index;
    }

    static inline GLint GetSize() {
      return Size;
    }

    static inline GLenum GetType() {
      return Type;
    }

    static inline GLboolean IsNormalized() {
      return Normalized;
    }

    static inline GLsizei GetStride() {
      return Stride;
    }

    static inline uword GetOffset() {
      return Offset;
    }

    static inline void
    Enable() {
      DLOG(INFO) << "enabling " << GetIndex() << " (" << GetSize() << ")";
      glEnableVertexAttribArray(GetIndex());
      CHECK_GL(FATAL);
    }

    static inline void
    Bind(const GLvoid* ptr) {
      glVertexAttribPointer(GetIndex(), GetSize(), GetType(), IsNormalized(), GetStride(), ptr);
      CHECK_GL(FATAL);
      Enable();
    }

    static inline void
    Bind() {
      return Bind((const GLvoid*) GetOffset());
    }
  };


  template<const GLuint Index, const uword Offset, const GLsizei Stride>
  class Vec2fVertexBufferAttribute : public VertexBufferAttribute<Index, 2, GL_FLOAT, GL_FALSE, Stride, Offset>{};

#define DEFINE_VEC2F_VERTEX_BUFFER_ATTR(Index, Offset, Stride, Name)                      \
  class Name##Attribute : public Vec2fVertexBufferAttribute<Index, Offset, Stride> {      \
  public:                                                                                 \
    static inline const char* GetName() { return #Name; }                                 \
  };

  template<const GLuint Index, 
           const GLsizei Stride,
           const uword Offset>
  class Vec3fVertexBufferAttribute : public VertexBufferAttribute<Index, 3, GL_FLOAT, GL_FALSE, Stride, Offset>{};

#define DEFINE_VEC3F_VERTEX_BUFFER_ATTR(Index, Offset, Stride, Name)                      \
  class Name##Attribute : public Vec3fVertexBufferAttribute<Index, Stride, Offset> {      \
  public:                                                                                 \
    static inline const char* GetName() { return #Name; }                                 \
  };

  template<const GLuint Index, const GLsizei Stride, const uword Offset>
  class Vec4fVertexBufferAttribute : public VertexBufferAttribute<Index, 4, GL_FLOAT, GL_FALSE, Stride, Offset>{};

#define DEFINE_VEC4F_VERTEX_BUFFER_ATTR(Index, Offset, Stride, Name)                      \
  class Name##Attribute : public Vec4fVertexBufferAttribute<Index, Stride, Offset> {      \
  public:                                                                                 \
    static inline const char* GetName() { return #Name; }                                 \
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
    VertexBufferTemplate(const VertexBufferTemplate& rhs):
      VertexBufferObject(rhs) {
    }
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
      return sizeof(Vertex);
    }
  };
}

#endif //MCC_VERTEX_BUFFER_H