#ifndef MCC_TEXTURE_TARGET_H
#define MCC_TEXTURE_TARGET_H

namespace mcc::texture {
  enum TextureTarget : GLenum {
    k1D = GL_TEXTURE_1D,
    k2D = GL_TEXTURE_2D,
    k3D = GL_TEXTURE_3D,
    kCubeMap = GL_TEXTURE_CUBE_MAP,

    kDefaultTarget = k2D,
  };

  static inline std::ostream& operator<<(std::ostream& stream, const TextureTarget& rhs) {
    switch(rhs) {
      case k1D: return stream << "1D";
      case k2D: return stream << "2D";
      case k3D: return stream << "3D";
      case kCubeMap: return stream << "CubeMap";
      default:
        return stream << "Unknown";
    }
  }
}

#endif //MCC_TEXTURE_TARGET_H