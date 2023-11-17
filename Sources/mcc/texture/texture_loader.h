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

    explicit TextureFileLoader(const res::Tag& tag, 
                               const std::string& filename,
                               const TextureFilter& filter,
                               const PixelStoreAlignment& alignment):
      FileLoader<Texture>(tag, filename),
      filter_(filter),
      alignment_(alignment) {
    }
    explicit TextureFileLoader(const res::Tag& tag, 
                               const std::string& filename):
      FileLoader<Texture>(tag, filename),
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
      const auto ptr = resource::Pointer(tag_, (uword) texture.value());
      return TextureRef(ptr);
    }
  };

  class PngFileLoader : public TextureFileLoader {
  protected:
    std::optional<Texture*> LoadTexture() override;
  public:
    explicit PngFileLoader(const res::Tag& tag, 
                           const std::string& filename):
      TextureFileLoader(tag, filename) {
    }
    explicit PngFileLoader(const res::Tag& tag, 
                           const std::string& filename,
                           const TextureFilter& filter,
                           const PixelStoreAlignment& alignment):
      TextureFileLoader(tag, filename, filter, alignment) {
    }
    ~PngFileLoader() override = default;
  };

  class JpegFileLoader : public TextureFileLoader {
  protected:
    std::optional<Texture*> LoadTexture() override;
  public:
    explicit JpegFileLoader(const res::Tag& tag, 
                            const std::string& filename,
                            const TextureFilter& filter,
                            const PixelStoreAlignment& alignment):
      TextureFileLoader(tag, filename, filter, alignment) {
    }
    explicit JpegFileLoader(const res::Tag& tag, 
                            const std::string& filename):
      TextureFileLoader(tag, filename) {
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
    explicit JsonTextureLoader(const res::Tag& tag, const std::string& filename):
      JsonFileLoader<Texture>(tag, filename) {
    }
    ~JsonTextureLoader() override = default;
    TextureRef Load() override;
  };

  class TextureLoader : public res::Loader<Texture> {
  private:
    std::string filename_;
  public:
    TextureLoader(const res::Tag& tag, const std::string& filename):
      Loader(tag),
      filename_(filename) {
    }
    ~TextureLoader() override = default;
    TextureRef Load() override;
  };
}

#endif //MCC_TEXTURE_LOADER_H