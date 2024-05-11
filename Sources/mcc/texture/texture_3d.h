#ifndef MCC_TEXTURE_H
#error "Please #include <mcc/texture/texture.h> instead."
#endif //MCC_TEXTURE_H

#ifndef MCC_TEXTURE_3D_H
#define MCC_TEXTURE_3D_H

#include "mcc/texture/texture.h"

namespace mcc {
  namespace texture {
    class Texture3d : public TextureTemplate<k3D> {
    protected:
      explicit Texture3d(const TextureId id):
        TextureTemplate<k3D>(id) {
      }
    public:
      ~Texture3d() override = default;
      std::string ToString() const override;
    public:
      static Texture3d* New(const json::TextureValue* value);
      static TextureRef New(const uri::Uri& uri);
      
      static inline TextureRef
      New(const uri::basic_uri& uri) {
        return New(uri::Uri(uri));
      }
    };
  }
  using texture::Texture3d;
}

#endif //MCC_TEXTURE_3D_H