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