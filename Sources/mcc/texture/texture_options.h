#ifndef MCC_TEXTURE_OPTIONS_H
#define MCC_TEXTURE_OPTIONS_H

#include <glog/logging.h>

#include "mcc/uri.h"
#include "mcc/texture/texture_wrap.h"
#include "mcc/texture/texture_target.h"
#include "mcc/texture/texture_filter.h"
#include "mcc/texture/texture_alignment.h"

namespace mcc::texture {
  class TextureOptions {
    friend class Texture;
  private:
    PixelStoreAlignment alignment_;
    TextureFilter filter_;
    TextureWrap wrap_;
    bool mipmap_;

    void ApplyTo(const TextureTarget target) {
      alignment_.Apply();
      filter_.ApplyTo(target);
      wrap_.ApplyTo(target);
      if(mipmap_) {
        glGenerateMipmap(target);
        CHECK_GL(FATAL);
      }
    }
  public:
    TextureOptions():
      alignment_(),
      filter_(),
      wrap_(),
      mipmap_(false) {
    }
    TextureOptions(const PixelStoreAlignment alignment,
                   const TextureFilter filter,
                   const TextureWrap wrap,
                   const bool mipmap = false):
      alignment_(alignment),
      filter_(filter),
      wrap_(wrap),
      mipmap_(mipmap) {
    }
    TextureOptions(const TextureOptions& rhs) = default;
    virtual ~TextureOptions() = default;

    PixelStoreAlignment alignment() const {
      return alignment_;
    }

    TextureFilter filter() const {
      return filter_;
    }

    TextureWrap wrap() const {
      return wrap_;
    }

    bool mipmap() const {
      return mipmap_;
    }

    TextureOptions& operator=(const TextureOptions& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const TextureOptions& rhs) {
      stream << "TextureOptions(";
      stream << "alignment=" << rhs.alignment() << ", ";
      stream << "filter=" << rhs.filter() << ", ";
      stream << "wrap=" << rhs.wrap() << ", ";
      stream << "mipmap=" << rhs.mipmap();
      stream << ")";
      return stream;
    }
  };

  class TextureOptionsFactory {
  protected:
    TextureOptionsFactory() = default;
  public:
    virtual ~TextureOptionsFactory() = default;
    virtual bool CreateTextureOptions(const uri::Uri& uri, TextureOptions& results) = 0;
  public:
    static TextureOptionsFactory* GetInstance();

    static inline bool
    Create(const uri::Uri& uri, TextureOptions& results) {
      const auto instance = GetInstance();
      MCC_ASSERT(instance != nullptr);
      return instance->CreateTextureOptions(uri, results);
    }
  };
}

#endif //MCC_TEXTURE_OPTIONS_H