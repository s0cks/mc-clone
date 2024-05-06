#include "mcc/texture/cube_map_builder.h"
#include "mcc/texture/texture.h"

namespace mcc::texture {
  void CubeMapBuilder::SetFace(const CubeMapFace face, const img::Image* image, const int level, const int border) {
    MCC_ASSERT(image);
    switch(image->type()) {
      case img::kRGB: {
        const auto data = FaceData {
          .face = face,
          .border = border,
          .level = level,
          .bytes = image->data()->bytes(),
          .num_bytes = image->data()->num_bytes(),
          .internal_format = kRGB,
          .format = kRGB,
          .type = GL_UNSIGNED_BYTE,
          .size = image->size(),
        };
        const auto result = faces_.insert(data);
        LOG_IF(ERROR, !result.second) << "failed to insert: " << data;
        break;
      }
      case img::kRGBA: {
        const auto data = FaceData {
          .face = face,
          .border = border,
          .level = level,
          .bytes = image->data()->bytes(),
          .num_bytes = image->data()->num_bytes(),
          .internal_format = kRGBA,
          .format = kRGBA,
          .type = GL_UNSIGNED_BYTE,
          .size = image->size(),
        };
        const auto result = faces_.insert(data);
        LOG_IF(ERROR, !result.second) << "failed to insert: " << data;
        break;
      }
      default:
        LOG(ERROR) << "invalid image type: " << image->type();
        break;
    }
  }

  void CubeMapBuilder::InitTexture(const TextureId id) const {
    MCC_ASSERT(IsValidTextureId(id));
    for(const auto& face : faces_) {
      glTexImage2D(
        face.face,
        face.level,
        face.internal_format,
        face.size[0],
        face.size[1],
        face.border,
        face.format,
        face.type,
        face.bytes
      );
    }
  }
}