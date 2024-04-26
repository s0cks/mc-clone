#ifndef MCC_IBO_USAGE_H
#define MCC_IBO_USAGE_H

#include "mcc/gfx.h"

namespace mcc::ibo {
#define FOR_EACH_IBO_USAGE(V)           \
  V(StreamDraw, GL_STREAM_DRAW)         \
  V(StreamRead, GL_STREAM_READ)         \
  V(StreamCopy, GL_STREAM_COPY)         \
  V(StaticDraw, GL_STATIC_DRAW)         \
  V(StaticRead, GL_STATIC_READ)         \
  V(StaticCopy, GL_STATIC_COPY)         \
  V(DynamicDraw, GL_DYNAMIC_DRAW)       \
  V(DynamicRead, GL_DYNAMIC_READ)       \
  V(DynamicCopy, GL_DYNAMIC_COPY)

  enum Usage : GLenum {
#define DEFINE_IBO_USAGE(Name, GlValue) k##Name = (GlValue),
    FOR_EACH_IBO_USAGE(DEFINE_IBO_USAGE)
#undef DEFINE_IBO_USAGE
    kDefaultUsage = kDynamicDraw,
  };

  static inline const char*
  ToString(const Usage rhs) {
    switch(rhs) {
#define DEFINE_TO_STRING(Name, GlValue) \
      case Usage::k##Name: return #Name;
      FOR_EACH_IBO_USAGE(DEFINE_TO_STRING)
#undef DEFINE_TO_STRING
      default: return "Unknown";
    }
  }

  static inline std::ostream&
  operator<<(std::ostream& stream, const Usage& rhs) {
    return stream << ToString(rhs);
  }

  static inline bool
  IsStream(const Usage& rhs) {
    switch(rhs) {
      case kStreamCopy:
      case kStreamRead:
      case kStreamDraw:
        return true;
      default:
        return false;
    }
  }

  static inline bool
  IsStatic(const Usage& rhs) {
    switch(rhs) {
      case kStaticCopy:
      case kStaticRead:
      case kStaticDraw:
        return true;
      default:
        return false;
    }
  }

  static inline bool
  IsDynamic(const Usage& rhs) {
    switch(rhs) {
      case kDynamicRead:
      case kDynamicCopy:
      case kDynamicDraw:
        return true;
      default:
        return false;
    }
  }
}

#endif //MCC_IBO_USAGE_H