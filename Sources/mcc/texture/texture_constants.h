#ifndef MCC_TEXTURE_CONSTANTS_H
#define MCC_TEXTURE_CONSTANTS_H

#include "mcc/gfx.h"

namespace mcc::texture {
  typedef glm::u32vec2 TextureSize;
  typedef glm::u32vec2 TextureCoord;
  typedef uint32_t Pixel;

  typedef GLuint TextureId;
  static constexpr const TextureId kInvalidTextureId = 0;

#define FOR_EACH_TEXTURE_TARGET(V) \
  V(1D, 1D)                        \
  V(2D, 2D)                        \
  V(3D, 3D)                        \
  V(CubeMap, CUBE_MAP)

  enum TextureTarget : GLenum {
#define DEFINE_TEXTURE_TARGET(Name, Target) k##Name = GL_TEXTURE_##Target,
    FOR_EACH_TEXTURE_TARGET(DEFINE_TEXTURE_TARGET)
#undef DEFINE_TEXTURE_TARGET
    kDefaultTarget = k2D,
  };

  static inline std::ostream& operator<<(std::ostream& stream, const TextureTarget& rhs) {
    switch(rhs) {
#define DEFINE_TOSTRING(Name, Target) \
      case k##Name: return stream << #Name << "(GL_TEXTURE_" << #Target << ")";
      FOR_EACH_TEXTURE_TARGET(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
      default: return stream << "Unknown";
    }
  }
}

#endif //MCC_TEXTURE_CONSTANTS_H