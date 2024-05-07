#ifndef MCC_TEXTURE_H
#error "Please #include <mcc/texture/texture.h> instead."
#endif //MCC_TEXTURE_H

#ifndef MCC_CUBE_MAP_H
#define MCC_CUBE_MAP_H

#include "mcc/texture/texture.h"
#include "mcc/texture/cube_map_face.h"
#include "mcc/texture/cube_map_builder.h"

namespace mcc {
  namespace texture {
    class CubeMap : public TextureTemplate<kCubeMap> {
    protected:
      explicit CubeMap(const TextureId id);
    public:
      ~CubeMap() override;
      std::string ToString() const override;
    private:
      static inline CubeMap*
      New(const TextureId id) {
        return new CubeMap(id);
      }
    public:
      static inline CubeMap*
      New(const CubeMapBuilder& builder) {
        return New(builder.Build());
      }

      static CubeMap* New(const uri::Uri& uri);

      static inline CubeMap*
      New(const uri::basic_uri& uri) {
        return New(uri::Uri(uri));
      }
    };

    class CubeMapLoader : public TextureLoader<CubeMap> {
    protected:
      CubeMapLoader() = default;
    public:
      ~CubeMapLoader() override = default;
    };
  }
  using texture::CubeMap;
}

#endif //MCC_CUBE_MAP_H