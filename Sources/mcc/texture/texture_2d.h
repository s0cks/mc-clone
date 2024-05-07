#ifndef MCC_TEXTURE_H
#error "Please #include <mcc/texture/texture.h> instead."
#endif //MCC_TEXTURE_H

#ifndef MCC_TEXTURE_2D_H
#define MCC_TEXTURE_2D_H

#include "mcc/texture/texture.h"
#include "mcc/texture/texture_builder.h"

namespace mcc {
  namespace texture {
    class Texture2d : public TextureTemplate<k2D> {
    public:
      explicit Texture2d(const TextureId id):
        TextureTemplate<k2D>(id) {
      }
      ~Texture2d() override = default;
      std::string ToString() const override;
    private:
      static inline Texture2d*
      New(const TextureId id) {
        return new Texture2d(id);
      }
    public:
      static inline Texture2d*
      New(const TextureBuilder& builder) {
        return New(builder.Build());
      }

      static Texture2d* New(const uri::Uri& uri);
      
      static inline Texture2d*
      New(const uri::basic_uri& uri) {
        auto target = uri;
        if(!StartsWith(target, "texture"))
          target = fmt::format("texture://{0:s}", uri);
        return New(uri::Uri(target));
      }
    };

    class Texture2dLoader : public TextureLoader<Texture2d> {
    protected:
      Texture2dLoader() = default;
    public:
      ~Texture2dLoader() override = default;
    };
  }
  using texture::Texture2d;
}

#endif //MCC_TEXTURE_2D_H