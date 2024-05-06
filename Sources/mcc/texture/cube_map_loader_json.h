#ifndef MCC_CUBE_MAP_LOADER_JSON_H
#define MCC_CUBE_MAP_LOADER_JSON_H

#include "mcc/texture/texture.h"

namespace mcc::texture {
  class JsonCubeMapLoader : public CubeMapLoader {
  protected:
    uri::Uri uri_;
    std::string dir_;

    static inline std::string
    GetCubeMapDirectory(const uri::Uri& uri) {
      MCC_ASSERT(uri.HasScheme("file"));
      MCC_ASSERT(uri.HasExtension(".json"));
      const auto& path = uri.path;
      const auto slashpos = path.find_last_of('/');
      return path.substr(0, path.size() - (path.size() - slashpos));
    }
  public:
    explicit JsonCubeMapLoader(const uri::Uri& uri):
      CubeMapLoader(),
      uri_(uri),
      dir_(GetCubeMapDirectory(uri)) {
      MCC_ASSERT(uri.HasScheme("file"));
      MCC_ASSERT(uri.HasExtension(".json"));
    }
    ~JsonCubeMapLoader() override = default;

    const uri::Uri& GetUri() const {
      return uri_;
    }

    CubeMap* LoadTexture() const override;
  public:
    static inline CubeMap*
    Load(const uri::Uri& uri) {
      JsonCubeMapLoader loader(uri);
      return loader.LoadTexture();
    }

    static inline CubeMap*
    Load(const uri::basic_uri& uri) {
      auto target = uri;
      if(!StartsWith(target, "file://"))
        target = fmt::format("file://{0:s}", target);
      return Load(uri::Uri(target));
    }

    static inline rx::observable<CubeMap*>
    LoadAsync(const uri::Uri& uri) {
      return rx::observable<>::create<CubeMap*>([uri](rx::subscriber<CubeMap*> s) {
        JsonCubeMapLoader loader(uri);
        const auto cubemap = loader.LoadTexture();
        if(!cubemap) {
          const auto err = fmt::format("failed to load CubeMap from: {0:s}", (const std::string&) uri);
          return s.on_error(rx::util::make_error_ptr(err));
        }
        s.on_next(cubemap);
        s.on_completed();
      });
    }

    static inline rx::observable<CubeMap*>
    LoadAsync(const uri::basic_uri& uri) {
      return LoadAsync(uri::Uri(uri));
    }
  };
}

#endif //MCC_CUBE_MAP_LOADER_JSON_H