#ifndef MCC_GFX_H
#error "Please #include <mcc/gfx.h> instead."
#endif //MCC_GFX_H

#ifndef MCC_GFX_ERROR_H
#define MCC_GFX_ERROR_H

namespace mcc::gfx {
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
  gfx::GlError err;                                                                      \
  while((err = gfx::GetGlError()) != gfx::kNoGlError)                               \
    google::LogMessage(__FILE__, __LINE__, google::Severity).stream() << "encountered: " << err;    \
})

#else

#define CHECK_GL(Severity)

#endif//MCC_DEBUG
}

#endif //MCC_GFX_ERROR_H