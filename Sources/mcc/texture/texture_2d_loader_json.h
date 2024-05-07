#ifndef MCC_TEXTURE_2D_LOADER_JSON_H
#define MCC_TEXTURE_2D_LOADER_JSON_H

#include "mcc/texture/texture.h"
#include "mcc/texture/texture_json.h"

namespace mcc::texture {
  class JsonTexture2dLoader : public Texture2dLoader {
  protected:
    json::ConstTextureObject doc_;

    inline const json::ConstTextureObject& doc() const {
      return doc_;
    }
  public:
    explicit JsonTexture2dLoader(const json::ConstObject& doc):
      Texture2dLoader(),
      doc_(doc) {
    }
    ~JsonTexture2dLoader() override = default;
    
    Texture2d* LoadTexture() const override;
  public:
    static inline Texture2d* 
    Load(const json::ConstObject& doc) {
      JsonTexture2dLoader loader(doc);
      return loader.LoadTexture();
    }
  };
}

#endif //MCC_TEXTURE_2D_LOADER_JSON_H