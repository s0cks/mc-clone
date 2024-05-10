#ifndef MCC_GFX_H
#error "Please #include <mcc/gfx.h> instead."
#endif //MCC_GFX_H

#ifndef MCC_GFX_MISC_H
#define MCC_GFX_MISC_H

#include "mcc/gfx_error.h"

namespace mcc {
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
    static inline int
    GetContextFlags() {
      int flags;
      glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
      CHECK_GL(FATAL);
      return flags;
    }

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
}

#endif //MCC_GFX_MISC_H