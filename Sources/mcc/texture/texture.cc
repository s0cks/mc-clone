#include <regex>
#include <algorithm>
#include <functional>
#include <unordered_set>

#include "mcc/uri.h"
#include "mcc/flags.h"
#include "mcc/common.h"
#include "mcc/image/image.h"
#include "mcc/texture/texture.h"
#include "mcc/texture/texture_spec.h"

namespace mcc {
  using namespace texture;

  typedef rx::observable<Texture*> TextureObservable;

  static inline Texture*
  ZipTexture(const TextureId& id, TextureSpec* spec, const img::ImagePtr& image) {
    const auto target = spec->GetTarget().value_or(k2D);
    NOT_IMPLEMENTED(ERROR);
    return nullptr;
  }

  static inline TextureObservable
  LoadTextureFromImageFile(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("file"));
    MCC_ASSERT(uri.HasExtension());
    return Texture::GenerateTextureId()
      .zip(&ZipTexture, TextureSpec::New(uri), img::GetImage(uri));
  }

  static inline TextureObservable
  LoadTextureFromJsonFile(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("file"));
    MCC_ASSERT(uri.HasExtension(".json"));
    MCC_ASSERT(FileExists(uri));
    return rx::observable<>::error<Texture*>(rx::util::make_error_ptr(std::runtime_error("")));
  }

  static inline TextureObservable
  LoadTextureFromFile(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("file"));
    MCC_ASSERT(uri.HasExtension());
    if(uri.HasExtension(".json")) {
      return LoadTextureFromJsonFile(uri);
    } else if(uri.HasExtension(".png")) {
      return LoadTextureFromImageFile(uri);
    } else if(uri.HasExtension(".jpg", ".jpeg")) {
      return LoadTextureFromImageFile(uri);
    }

    const auto err = fmt::format("invalid texture: {0:s}", (const std::string&) uri);
    return rx::observable<>::error<Texture*>(std::runtime_error(err));
  }

  TextureRef GetTexture(const uri::Uri& uri) {
    if(!uri.HasScheme("tex", "texture")) {
      LOG(WARNING) << "invalid texture uri: " << uri;
      return TextureRef();
    }

    const uri::Uri file(fmt::format("file://{0:s}/textures/{1:s}", FLAGS_resources, uri.path));
    
    DLOG(INFO) << "getting texture: " << file;
    const auto texture = LoadTextureFromFile(file)
      .as_blocking()
      .first();
    return TextureRef(texture);
  }

  namespace texture {
    rx::observable<TextureId> Texture::GenerateTextureId(const int num) {
      MCC_ASSERT(num >= 1);
      return rx::observable<>::create<TextureId>([num](rx::subscriber<TextureId> s) {
        TextureId ids[num];
        glGenTextures(num, ids);
        CHECK_GL(FATAL);

        for(auto idx = 0; idx < num; idx++)
          s.on_next(ids[idx]);
        s.on_completed();
      });
    }

    TextureWrap Texture::GetTextureWrap() const {
      Bind(0);
      const auto wrap = TextureWrap::GetTextureWrap<k2D>();
      Unbind();
      return wrap;
    }

    void Texture::Destroy() {
      glDeleteTextures(1, &id_);
      CHECK_GL(FATAL);
    }
  }
}