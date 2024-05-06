#ifndef MCC_TEXTURE_2D_LOADER_FILE_H
#define MCC_TEXTURE_2D_LOADER_FILE_H

#include "mcc/texture/texture.h"

namespace mcc::texture {
  class Texture2dFileLoader : public Texture2dLoader {
  protected:
    const uri::Uri& uri_;
  public:
    explicit Texture2dFileLoader(const uri::Uri& uri):
      Texture2dLoader(),
      uri_(uri) {
    }
    ~Texture2dFileLoader() override = default;
    Texture2d* LoadTexture() const override;
  public:
    static inline Texture2d*
    Load(const uri::Uri& uri) {
      Texture2dFileLoader loader(uri);
      return loader.LoadTexture();
    }

    static inline rx::observable<Texture2d*>
    LoadAsync(const uri::Uri& uri) {
      return rx::observable<>::create<Texture2d*>([uri](rx::subscriber<Texture2d*> s) {
        Texture2dFileLoader loader(uri);
        const auto texture = loader.LoadTexture();
        if(!texture) {
          const auto err = fmt::format("failed to load Texture2d from: {0:s}", (const std::string&) uri);
          return s.on_error(rx::util::make_error_ptr(std::runtime_error(err)));
        }
        s.on_next(texture);
        s.on_completed();
      });
    }
  };
}

#endif //MCC_TEXTURE_2D_LOADER_FILE_H