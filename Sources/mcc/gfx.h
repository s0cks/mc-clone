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

#include "mcc/glm.h"

#include <iostream>

#include <units.h>

#include <glog/logging.h>

#include "mcc/common.h"

namespace mcc {
#ifndef GL_STACK_OVERFLOW
#define GL_STACK_OVERFLOW 0x0503
#endif //GL_STACK_OVERFLOW

#ifndef GL_STACK_UNDERFLOW 
#define GL_STACK_UNDERFLOW 0x0504
#endif //GL_STACK_UNDERFLOW

#define FOR_EACH_GL_ERROR(V)                                          \
  V(InvalidEnum, GL_INVALID_ENUM)                                     \
  V(InvalidValue, GL_INVALID_VALUE)                                   \
  V(InvalidOperation, GL_INVALID_OPERATION)                           \
  V(StackOverflow, GL_STACK_OVERFLOW)                                 \
  V(StackUnderflow, GL_STACK_UNDERFLOW)                               \
  V(OutOfMemory, GL_OUT_OF_MEMORY)                                    \
  V(InvalidFramebufferOperation, GL_INVALID_FRAMEBUFFER_OPERATION)

enum GlError : GLenum {
#define DEFINE_GL_ERROR(Name, GlValue) kGl##Name##Error = (GlValue),
  FOR_EACH_GL_ERROR(DEFINE_GL_ERROR)
#undef DEFINE_GL_ERROR
  kNumberOfGlErrors, //TODO: investigate
  kNoGlError = GL_NO_ERROR,
};

static inline std::ostream& operator<<(std::ostream& stream, const GlError& rhs) {
  switch(rhs) {
#define TO_STRING(Name, GlValue) \
    case GlError::kGl##Name##Error: return stream << "Gl" #Name "Error";
    FOR_EACH_GL_ERROR(TO_STRING)
#undef TO_STRING
    default: return stream << "unknown GlError: " << static_cast<GLenum>(rhs);
  }
}

static inline GlError
GetGlError() {
  return static_cast<GlError>(glGetError());
}

#ifdef MCC_DEBUG

#define CHECK_GL(Severity)({                                                        \
  GlError err;                                                                      \
  while((err = GetGlError()) != kNoGlError)                                         \
    google::LogMessage(__FILE__, __LINE__, google::Severity).stream() << "encountered: " << err;    \
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

      bool IsInverted() const {
        return Inverted;
      }
    };
    
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

  using gfx::DepthTestScope;
  using gfx::InvertedDepthTestScope;

  using gfx::ScissorTestScope;
  using gfx::InvertedScissorTestScope;

  using gfx::BlendTestScope;
  using gfx::InvertedBlendTestScope;

#define DEFINE_RESOURCE_SCOPE(Resource) \
  typedef gfx::BindScope<Resource> Resource##Scope;

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