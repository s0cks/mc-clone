#ifndef MCC_CUBE_MAP_BUILDER_H
#define MCC_CUBE_MAP_BUILDER_H

#include <set>
#include <units.h>
#include "mcc/image/image.h"
#include "mcc/texture/cube_map_face.h"
#include "mcc/texture/texture_builder.h"

namespace mcc::texture {
  class CubeMapBuilder : public TextureBuilderBase {
  public:
    struct FaceData {
      CubeMapFace face;
      TextureFormat internal_format;
      TextureFormat format;
      GLenum type;
      const uint8_t* bytes;
      uint64_t num_bytes;
      glm::vec2 size;
      int level;
      int border;

      FaceData() = default;
      FaceData(const FaceData& rhs) = default;
      ~FaceData() = default;

      FaceData& operator=(const FaceData& rhs) = default;

      bool operator==(const FaceData& rhs) const {
        return face == rhs.face;
      }

      bool operator!=(const FaceData& rhs) const {
        return face != rhs.face;
      }

      bool operator<(const FaceData& rhs) const {
        return face < rhs.face;
      }

      bool operator>(const FaceData& rhs) const {
        return face > rhs.face;
      }

      friend std::ostream& operator<<(std::ostream& stream, const FaceData& rhs) {
        using namespace units::data;
        stream << "FaceData(";
        stream << "face=" << rhs.face;
        stream << "internal_format=" << rhs.internal_format << ", ";
        stream << "format=" << rhs.format << ", ";
        stream << "type+" << rhs.type << ", ";
        stream << "bytes=" << (const void*) rhs.bytes << ", ";
        stream << "num_bytes=" << byte_t(rhs.num_bytes) << ", ";
        stream << "size=" << glm::to_string(rhs.size) << ", ";
        stream << "level=" << rhs.level << ", ";
        stream << "border=" << rhs.border;
        stream << ")";
        return stream;
      }
    };
  protected:
    std::set<FaceData> faces_;

    void InitTexture(const TextureId id) const override;
  public:
    CubeMapBuilder():
      TextureBuilderBase(kCubeMap),
      faces_() {
    }
    ~CubeMapBuilder() override = default;

    void SetFace(const CubeMapFace face, const img::Image* data, const int level = 0, const int border = 0);

    inline void SetTop(const img::Image* data, const int level = 0, const int border = 0) {
      return SetFace(CubeMapFace::kTop, data, level, border);
    }

    inline void SetBottom(const img::Image* data, const int level = 0, const int border = 0) {
      return SetFace(CubeMapFace::kBottom, data, level, border);
    }

    inline void SetBack(const img::Image* data, const int level = 0, const int border = 0) {
      return SetFace(CubeMapFace::kBack, data, level, border);
    }

    inline void SetFront(const img::Image* data, const int level = 0, const int border = 0) {
      return SetFace(CubeMapFace::kFront, data, level, border);
    }

    inline void SetLeft(const img::Image* data, const int level = 0, const int border = 0) {
      return SetFace(CubeMapFace::kLeft, data, level, border);
    }

    inline void SetRight(const img::Image* data, const int level = 0, const int border = 0) {
      return SetFace(CubeMapFace::kRight, data, level, border);
    }
  };
}

#endif //MCC_CUBE_MAP_BUILDER_H