#ifndef MCC_GFX_DRAW_MODE_H
#define MCC_GFX_DRAW_MODE_H

#include "mcc/gfx.h"

namespace mcc::gfx {
  enum DrawMode : GLenum {
    kPoints = GL_POINTS,
    kLineStrip = GL_LINE_STRIP,
    kLineLoop = GL_LINE_LOOP,
    kLines = GL_LINES,
    kLineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
    kLinesAdjacency = GL_LINES_ADJACENCY,
    kTriangleStrip = GL_TRIANGLE_STRIP,
    kTriangleFan = GL_TRIANGLE_FAN,
    kTriangles = GL_TRIANGLES,
    kTriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
    kTrianglesAdjacency = GL_TRIANGLES_ADJACENCY,
    kPatches = GL_PATCHES,

    kNumberOfDrawModes = 12,
  };
}

#endif //MCC_GFX_DRAW_MODE_H