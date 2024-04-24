#ifndef MCC_VBO_USAGE_H
#define MCC_VBO_USAGE_H

#include "mcc/gfx.h"

namespace mcc::vbo {
#define FOR_EACH_VBO_USAGE(V)         \
  V(StreamDraw,   GL_STREAM_DRAW)     \
  V(StreamRead,   GL_STREAM_READ)     \
  V(StreamCopy,   GL_STREAM_COPY)     \
  V(StaticDraw,   GL_STATIC_DRAW)     \
  V(StaticRead,   GL_STATIC_READ)     \
  V(StaticCopy,   GL_STATIC_COPY)     \
  V(DynamicDraw,  GL_DYNAMIC_READ)    \
  V(DynamicRead,  GL_DYNAMIC_READ)    \
  V(DynamicCopy,  GL_DYNAMIC_COPY)

  enum Usage : GLenum {
#define DEFINE_VBO_USAGE(Name, GlValue) k##Name = (GlValue),
    FOR_EACH_VBO_USAGE(DEFINE_VBO_USAGE)
#undef DEFINE_VBO_USAGE
  };
}

#endif //MCC_VBO_USAGE_H