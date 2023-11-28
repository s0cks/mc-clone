#ifndef MCC_TEXTURE_LOADER_H
#define MCC_TEXTURE_LOADER_H

#include "mcc/texture/image.h"
#include "mcc/texture/texture.h"

namespace mcc::texture {
  class TextureFileLoader : public res::FileLoader<Texture> {
  protected:
    Image image_;
    TextureFilter filter_;
    PixelStoreAlignment alignment_;

    explicit TextureFileLoader(const std::string& filename,
                               const TextureFilter& filter,
                               const PixelStoreAlignment& alignment):
      FileLoader<Texture>(filename),
      filter_(filter),
      alignment_(alignment) {
    }
    explicit TextureFileLoader(const std::string& filename):
      FileLoader<Texture>(filename),
      filter_(),
      alignment_() {
    }

    virtual std::optional<Texture*> LoadTexture() = 0;
  public:
    ~TextureFileLoader() override = default;

    TextureRef Load() override {
      const auto texture = LoadTexture();
      if(!texture) {
        DLOG(ERROR) << "failed to load texture from: " << filename_;
        return TextureRef();
      }
      return TextureRef(texture.value());
    }
  };

  class PngFileLoader : public TextureFileLoader {
  protected:
    std::optional<Texture*> LoadTexture() override;
  public:
    explicit PngFileLoader(const std::string& filename):
      TextureFileLoader(filename) {
    }
    explicit PngFileLoader(const std::string& filename,
                           const TextureFilter& filter,
                           const PixelStoreAlignment& alignment):
      TextureFileLoader(filename, filter, alignment) {
    }
    ~PngFileLoader() override = default;
  };

  class JpegFileLoader : public TextureFileLoader {
  protected:
    std::optional<Texture*> LoadTexture() override;
  public:
    explicit JpegFileLoader(const std::string& filename,
                            const TextureFilter& filter,
                            const PixelStoreAlignment& alignment):
      TextureFileLoader(filename, filter, alignment) {
    }
    explicit JpegFileLoader(const std::string& filename):
      TextureFileLoader(filename) {
    }
    ~JpegFileLoader() override = default;
  };

  class JsonTextureLoader : public res::JsonFileLoader<Texture> {
  protected:
    inline std::optional<std::string>
    GetDocumentTarget() {
      return GetDocumentString("target");
    }

    inline std::optional<std::string>
    GetDocumentTexture() {
      return GetDocumentString("texture");
    }
  public:
    explicit JsonTextureLoader(const std::string& filename):
      JsonFileLoader<Texture>(filename) {
    }
    ~JsonTextureLoader() override = default;
    TextureRef Load() override;
  };

  class TextureLoader : public res::Loader<Texture> {
  private:
    const uri::Uri& target_;
  public:
    TextureLoader(const uri::Uri& target):
      Loader(),
      target_(target) {
    }
    ~TextureLoader() override = default;

    const uri::Uri& target() const {
      return target_;
    }

    TextureRef Load() override;
  };
}

#endif //MCC_TEXTURE_LOADER_H