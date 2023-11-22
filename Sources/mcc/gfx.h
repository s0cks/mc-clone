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
    struct Resource {
      Resource() = default;
      virtual ~Resource() = default;
      virtual void Bind() const = 0;
      virtual void Unbind() const = 0;
      virtual void Delete() = 0;
    };

    template<const GLenum Capability, const bool Inverted = false>
    class CapabilityScope {
    public:
      CapabilityScope() {
        if(Inverted) {
          glDisable(Capability);
        } else {
          glEnable(Capability);
        }
        CHECK_GL(FATAL);
      }
      virtual ~CapabilityScope() {
        if(Inverted) {
          glEnable(Capability);
        } else {
          glDisable(Capability);
        }
        CHECK_GL(FATAL);
      }
    };
    
    template<const bool Inverted = false>
    class CullFaceCapabilityScope : public CapabilityScope<GL_CULL_FACE, Inverted>{};
    typedef CullFaceCapabilityScope<false> CullFaceScope;
    typedef CullFaceCapabilityScope<true> InvertedCullFaceScope;

    enum GlFunction : GLenum {
      kNever = GL_NEVER,
      kLess = GL_LESS,
      kEqual = GL_EQUAL,
      kLequal = GL_LEQUAL,
      kGreater = GL_GREATER,
      kNotEqual = GL_NOTEQUAL,
      kGequal = GL_GEQUAL,
      kAlways = GL_ALWAYS
    };
    typedef GlFunction DepthFunction;

    static constexpr const DepthFunction kDefaultDepthFunction = kLess;

    static inline DepthFunction GetCurrentDepthFunction() {
      GLint value;
      glGetIntegerv(GL_DEPTH_FUNC, &value);
      CHECK_GL(FATAL);
      return static_cast<DepthFunction>(value);
    }
    
    template<const bool Inverted = false>
    class DepthTestCapabilityScope : public CapabilityScope<GL_DEPTH_TEST, Inverted> {
    private:
      DepthFunction previous_;
      bool restore_;
    public:
      DepthTestCapabilityScope(const DepthFunction func = kDefaultDepthFunction, const bool restore = true):
        CapabilityScope<GL_DEPTH_TEST, Inverted>(),
        previous_(GetCurrentDepthFunction()),
        restore_(restore) {
        glDepthFunc(func);
        CHECK_GL(FATAL);
      }
      ~DepthTestCapabilityScope() override {
        if(restore_) {
          glDepthFunc(previous_);
          CHECK_GL(FATAL);
        }
      }
    };
    typedef DepthTestCapabilityScope<false> DepthTestScope;
    typedef DepthTestCapabilityScope<true> InvertedDepthTestScope;

    template<const bool Inverted = false>
    class ScissorTestCapabilityScope : public CapabilityScope<GL_SCISSOR_TEST, Inverted> {
    public:
      ScissorTestCapabilityScope():
        CapabilityScope<GL_SCISSOR_TEST, Inverted>() {
      }
      ~ScissorTestCapabilityScope() override = default;
    };
    typedef ScissorTestCapabilityScope<false> ScissorTestScope;
    typedef ScissorTestCapabilityScope<true> InvertedScissorTestScope;

    enum BlendSFactor : GLenum {
      kSrcAlpha = GL_SRC_ALPHA,
    };

    enum BlendDFactor : GLenum {
      kOneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA,
    };

    enum BlendEquation : GLenum {
      kAddFunc = GL_FUNC_ADD,
    };

    static inline void
    GetCurrentBlendFunction(GLint* srgb, GLint* sa, GLint* drgb, GLint* da) {
      glGetIntegerv(GL_BLEND_SRC_RGB, srgb);
      CHECK_GL(FATAL);
      glGetIntegerv(GL_BLEND_SRC_ALPHA, sa);
      CHECK_GL(FATAL);
      glGetIntegerv(GL_BLEND_DST_RGB, drgb);
      CHECK_GL(FATAL);
      glGetIntegerv(GL_BLEND_DST_ALPHA, da);
      CHECK_GL(FATAL);
    }

    template<const bool Inverted = false>
    class BlendTestCapabilityScope : public CapabilityScope<GL_BLEND, Inverted> {
    private:
      bool restore_;
      GLint srgb_;
      GLint sa_;
      GLint drgb_;
      GLint da_;
    public:
      BlendTestCapabilityScope(const BlendSFactor sfactor,
                               const BlendDFactor dfactor,
                               const BlendEquation equation,
                               const bool restore = true):
        CapabilityScope<GL_BLEND, Inverted>(),
        restore_(restore) {
        glBlendFunc(sfactor, dfactor);
        CHECK_GL(FATAL);
        glBlendEquation(equation);
        CHECK_GL(FATAL);
        if(restore) {
          GetCurrentBlendFunction(&srgb_, &sa_, &drgb_, &da_);
        }
      }
      ~BlendTestCapabilityScope() override {
        if(restore_) {
          glBlendFuncSeparate(srgb_, drgb_, sa_, da_);
          CHECK_GL(FATAL);
        }
      }
    };
    typedef BlendTestCapabilityScope<false> BlendTestScope;
    typedef BlendTestCapabilityScope<true> InvertedBlendTestScope;

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

    class Renderable {
    protected:
      Renderable() = default;
      virtual void Render();
    public:
      virtual ~Renderable() = default;
    };
  }
  using gfx::CullFaceScope;
  using gfx::InvertedCullFaceScope;

  using gfx::DepthTestScope;
  using gfx::InvertedDepthTestScope;

  using gfx::ScissorTestScope;
  using gfx::InvertedScissorTestScope;

  using gfx::BlendTestScope;
  using gfx::InvertedBlendTestScope;

#define DEFINE_RESOURCE_SCOPE(Resource) \
  typedef gfx::BindScope<Resource> Resource##Scope;

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

  typedef glm::u64vec2 Dimension;

  template<const GLenum Target,
           const GLenum Access>
  class MappedBufferScope {
  protected:
    uword ptr_;

    static inline uword
    MapBuffer() {
      const auto ptr = glMapBuffer(Target, Access);
      CHECK_GL(FATAL);
      return ptr == NULL ? 0 : (uword) ptr;
    }
  public:
    explicit MappedBufferScope(const uword ptr = MapBuffer()):
      ptr_(ptr) {
    }
    virtual ~MappedBufferScope() {
      if(IsMapped()) {
        glUnmapBuffer(Target);
        CHECK_GL(FATAL);
      }
    }

    uword address() const {
      return ptr_;
    }

    inline void* raw_ptr() const {
      return (void*) address();
    }

    inline uint8_t* bytes() const {
      return (uint8_t) address();
    }

    inline bool IsMapped() const {
      return ptr_ != 0;
    }
  };

  template<const GLenum Target>
  class WriteOnlyMappedBufferScope : public MappedBufferScope<Target, GL_WRITE_ONLY> {
  public:
    explicit WriteOnlyMappedBufferScope(const uword ptr = MappedBufferScope<Target, GL_WRITE_ONLY>::MapBuffer()):
      MappedBufferScope<Target, GL_WRITE_ONLY>(ptr) {
    }
    ~WriteOnlyMappedBufferScope() override = default;
  };

  template<const GLenum Target>
  class ReadOnlyMappedBufferScope : public MappedBufferScope<Target, GL_READ_ONLY> {
  public:
    explicit ReadOnlyMappedBufferScope(const uword ptr = MappedBufferScope<Target, GL_READ_ONLY>::MapBuffer()):
      MappedBufferScope<Target, GL_READ_ONLY>(ptr) {
    }
    ~ReadOnlyMappedBufferScope() override = default;
  };
}

#ifdef M_PI
#define PI M_PI
#endif //M_PI

#ifndef PI
#define PI 3.141592654
#endif //PI

#endif //MCC_GFX_H