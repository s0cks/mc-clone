#ifndef MCC_TEXTURE_LOADER_H
#define MCC_TEXTURE_LOADER_H

#include <fmt/format.h>
#include "mcc/rx.h"

namespace mcc::texture {
  template<class T>
  class TextureLoader {
  protected:
    TextureLoader() = default;
  public:
    virtual ~TextureLoader() = default;
    virtual T* LoadTexture() const = 0;

    virtual rx::observable<T*> LoadTextureAsync() const {
      return rx::observable<>::create<T*>([this](rx::subscriber<T*> s) {
        const auto texture = LoadTexture();
        if(!texture) {
          const auto err = fmt::format("failed to LoadTexture.");
          return s.on_error(rx::util::make_error_ptr(std::runtime_error(err)));
        }
        s.on_next(texture);
        s.on_completed();
      });
    }
  };
}

#endif //MCC_TEXTURE_LOADER_H