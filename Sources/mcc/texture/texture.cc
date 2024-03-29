#include <regex>
#include <algorithm>
#include <functional>
#include <unordered_set>

#include "mcc/uri.h"
#include "mcc/flags.h"
#include "mcc/common.h"
#include "mcc/texture/texture.h"
#include "mcc/texture/image/image.h"
#include "mcc/texture/texture_loader.h"

namespace mcc {
  static const std::unordered_set<std::string> kValidTextureExtensions = {
    ".json",
    ".jpeg",
    ".jpg",
    ".png"
  };

  static inline bool
  IsValidTextureExtension(const std::string& ext) {
    const auto pos = kValidTextureExtensions.find(ext);
    return pos != kValidTextureExtensions.end();
  }

  static inline bool
  IsTextureScheme(const uri::Uri& uri) {
    return EqualsIgnoreCase(uri.scheme, "tex") || EqualsIgnoreCase(uri.scheme, "texture");
  }

  static inline bool
  HasValidTextureExtension(const uri::Uri& uri) {
    const auto ext = uri.GetPathExtension();
    return ext && IsValidTextureExtension((*ext));
  }

  static inline std::string
  GetTextureDirectory(const std::string& path) {
    return fmt::format("{0:s}/textures/{1:s}", FLAGS_resources, path);
  }

  static inline std::string
  GetTextureDirectory(const uri::Uri& uri) {
    return GetTextureDirectory(uri.path);
  }

  static inline TextureRef LoadTexture(const std::string& path) {
    auto target = path;
    if(!StartsWith(target, FLAGS_resources + "/textures"))
      target = fmt::format("{0:s}/textures/{1:s}", FLAGS_resources, target);
    if(IsDirectory(target))
      return LoadTexture(fmt::format("{0:s}/index.json"));
    return TextureRef();
  }

  TextureRef GetTexture(const uri::Uri& uri) {
    DLOG(INFO) << "checking for: " << uri;
    if(!IsTextureScheme(uri)) {
      DLOG(ERROR) << uri << " is not a texture.";
      return TextureRef();
    }
    
    return TextureRef(texture::TextureFactory::Create(uri));
  }

  namespace texture {
    TextureWrap Texture::GetTextureWrap() const {
      Bind(0);
      const auto wrap = TextureWrap::GetTextureWrap<k2D>();
      Unbind();
      return wrap;
    }

    static TextureFactory* factory_ = nullptr;

    class TextureFactoryImpl : public TextureFactory {
      friend class TextureFactory;
    protected:
      TextureFactoryImpl() = default;

      inline bool LoadTextureFromFile(const uri::Uri& uri, const std::string& file, TextureOptions& options, TextureData& data) {
        DLOG(INFO) << "loading " << uri << " from: " << file << "....";

        NOT_IMPLEMENTED(ERROR);
        return false;
      }

      inline bool LoadTextureFromDirectory(const uri::Uri& uri, const std::string& dir, TextureOptions& options, TextureData& data) {
        NOT_IMPLEMENTED(ERROR);
        return false;
      }

      inline bool LoadTexture(const uri::Uri& uri, TextureOptions& options, TextureData& data) {
        if(!IsTextureScheme(uri)) {
          DLOG(ERROR) << "not a texture Uri: " << uri;
          return false;
        }

        const auto path = fmt::format("{0:s}/textures/{1:s}", FLAGS_resources, uri.path);
        if(IsDirectory(path)) {
          return LoadTextureFromDirectory(uri, path, options, data);
        } else if(HasValidTextureExtension(path)) {
          return LoadTextureFromFile(uri, path, options, data);
        }

        for(const auto& ext : kValidTextureExtensions) {
          const auto new_path = fmt::format("{0:s}.{1:s}", path, ext);
          if(FileExists(new_path))
            return LoadTextureFromFile(uri, new_path, options, data);
        }

        DLOG(ERROR) << "failed to load texture from: " << uri;
        return false;
      }
    public:
      ~TextureFactoryImpl() override = default;
      
      Texture* CreateTexture(const TextureTarget target, const uri::Uri& uri) override {
        TextureId id;
        glGenTextures(1, &id);
        CHECK_GL(FATAL);
        glBindTexture(target, id);
        CHECK_GL(FATAL);
        //TODO:
        
        TextureOptions options;
        LOG_IF(WARNING, !TextureOptionsFactory::Create(uri, options)) << "failed to create TextureOptions for: " << uri;
        // apply options

        TextureData data;
        // load pixels
      
        glBindTexture(target, 0);
        CHECK_GL(FATAL);
        return new Texture(id, options, data);
      }
    };

    TextureFactory* TextureFactory::GetInstance() {
      if(factory_ == nullptr)
        factory_ = new TextureFactoryImpl();
      return factory_;
    }
  }
}