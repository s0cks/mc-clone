#ifndef MCC_SHADER_CACHE_H
#define MCC_SHADER_CACHE_H

#include "mcc/flags.h"
#include "mcc/common.h"
#include "mcc/shader/shader.h"

namespace mcc::shader {
  static constexpr const char* kDefaultShaderCacheDir = "";
  DECLARE_string(shader_cache_dir);

  static constexpr const uint32_t kDefaultShaderCacheSize = 128;
  DECLARE_uint32(shader_cache_size);

  static constexpr const uint32_t kDefaultShaderCacheBuckets = 10;
  DECLARE_uint32(shader_cache_buckets);

  class CacheLoader {
  protected:
    CacheLoader() = default;
  public:
    virtual ~CacheLoader() = default;
    virtual Shader Load(const std::string& k) const = 0;
  };

  class BasicCacheLoader : public CacheLoader {
    friend class Cache;
  private:
    BasicCacheLoader() = default;
  public:
    ~BasicCacheLoader() override = default;

    Shader Load(const std::string& k) const override;
  };

  class DefaultCacheLoader : public CacheLoader {
    friend class Cache;
  private:
    const Shader& defaultValue_;

    DefaultCacheLoader(const Shader& defaultValue):
      CacheLoader(),
      defaultValue_(defaultValue) {
    }
  public:
    ~DefaultCacheLoader() override = default;

    Shader Load(const std::string& k) const override {
      return defaultValue_;
    }
  };

  static constexpr const char* kVertexShaderExtension = ".vsh";
  static constexpr const char* kFragmentShaderExtension = ".fsh";

  class Cache {
    DEFINE_NON_INSTANTIABLE_TYPE(Cache);
  public:
    static void Init();

    static inline std::string
    GetCacheDirectory() {
      if(FLAGS_shader_cache_dir.empty())
        return FLAGS_resources + "/shaders";
      return FLAGS_shader_cache_dir;
    }

    static inline uint32_t
    GetCapacity() {
      return FLAGS_shader_cache_size;
    }

    static uint32_t GetSize();
    static Shader Get(const std::string& k, CacheLoader* loader);

    template<class CacheLoader = BasicCacheLoader>
    static Shader Get(const std::string& k) {
      CacheLoader loader;
      return Get(k, &loader);
    }

    static Shader GetOrDefault(const std::string& k, const Shader& defaultValue) {
      DefaultCacheLoader loader(defaultValue);
      return Get(k, &loader);
    }
  };
}

#endif //MCC_SHADER_CACHE_H