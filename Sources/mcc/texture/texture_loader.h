#ifndef MCC_TEXTURE_LOADER_H
#define MCC_TEXTURE_LOADER_H

#include "mcc/texture/texture.h"

namespace mcc::texture {
  class TextureLoader {
  protected:
    TextureLoader() = default;
  public:
    virtual ~TextureLoader() = default;
    virtual Texture Load() = 0;
  };

  class TextureFileLoader : public TextureLoader {
  protected:
    std::string filename_;

    explicit TextureFileLoader(const std::string& filename):
      TextureLoader(),
      filename_(filename) {
    }
  public:
    ~TextureFileLoader() override = default;
  };

  class PngTextureLoader : public TextureFileLoader {
  public:
    explicit PngTextureLoader(const std::string& filename):
      TextureFileLoader(filename) {
    }
    ~PngTextureLoader() override = default;
    Texture Load() override;
  };

  class JpegTextureLoader : public TextureFileLoader {
  public:
    explicit JpegTextureLoader(const std::string& filename):
      TextureFileLoader(filename) {
    }
    ~JpegTextureLoader() override = default;
    Texture Load() override;
  };
}

#endif //MCC_TEXTURE_LOADER_H