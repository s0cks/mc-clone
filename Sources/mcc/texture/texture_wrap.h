#ifndef MCC_TEXTURE_WRAP_H
#define MCC_TEXTURE_WRAP_H

#include <string>

#include "mcc/gfx.h"
#include "mcc/json.h"
#include "mcc/texture/texture_target.h"

namespace mcc::texture {
  enum TextureWrapMode : GLenum {
    kRepeat = GL_REPEAT,
    kClampToEdge = GL_CLAMP_TO_EDGE,
    kClampToBorder = GL_CLAMP_TO_BORDER,

    kDefaultWrapMode = kClampToEdge,
  };

  struct TextureWrap {
    TextureWrapMode r;
    TextureWrapMode s;
    TextureWrapMode t;

    constexpr TextureWrap(const TextureWrapMode R,
                          const TextureWrapMode S,
                          const TextureWrapMode T):
      r(R),
      s(S),
      t(T) {
    }
    explicit constexpr TextureWrap(const TextureWrapMode mode = kDefaultWrapMode):
      TextureWrap(mode, mode, mode) {
    }
    TextureWrap(const TextureWrap& rhs) = default;
    ~TextureWrap() = default;

    void ApplyTo(const TextureTarget target) const {
      glTexParameteri(target, GL_TEXTURE_WRAP_R, r);
      CHECK_GL(FATAL);
      glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
      CHECK_GL(FATAL);
      glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
      CHECK_GL(FATAL);
    }

    TextureWrap& operator=(const TextureWrap& rhs) = default;

    TextureWrap& operator=(const TextureWrapMode& rhs) {
      r = s = t = rhs;
      return *this;
    }

    friend std::ostream& operator<<(std::ostream& stream, const TextureWrap& rhs) {
      stream << "TextureWrap(";
      stream << "r=" << rhs.r << ", ";
      stream << "s=" << rhs.s << ", ";
      stream << "t=" << rhs.t;
      stream << ")";
      return stream;
    }

    bool operator==(const TextureWrap& rhs) const {
      return r == rhs.r
          && s == rhs.s
          && t == rhs.t;
    }

    bool operator==(const TextureWrapMode& rhs) const {
      return r == rhs
          && s == rhs
          && t == rhs;
    }

    bool operator!=(const TextureWrap& rhs) const {
      return r != rhs.r
          || s != rhs.s
          || t != rhs.t;
    }

    bool operator!=(const TextureWrapMode& rhs) const {
      return r != rhs
          || s != rhs
          || t != rhs;
    }
  };

  static constexpr const auto kDefaultWrap = TextureWrap(kDefaultWrapMode);
  static constexpr const auto kRepeatWrap = TextureWrap(kRepeat);
  static constexpr const auto kClampToEdgeWrap = TextureWrap(kClampToEdge);
  static constexpr const auto kClampToBorderWrap = TextureWrap(kClampToBorder);

  static inline std::optional<TextureWrapMode>
  ParseTextureWrapMode(const std::string& value) {
    if(value.empty() || value.length() <= 0)
      return std::nullopt;

    if(value == "default") {
      return { kDefaultWrapMode };
    } else if(value == "repeat") {
      return { kRepeat };
    } else if(value == "clamp2edge") {
      return { kClampToEdge };
    } else if(value == "clamp2border") {
      return { kClampToBorder };
    }

    DLOG(ERROR) << "unknown TextureWrapMode: " << value;
    return std::nullopt;
  }

  class JsonTextureWrap {
    DEFINE_NON_COPYABLE_TYPE(JsonTextureWrap);
    static constexpr const auto kRFieldName = "r";
    static constexpr const auto kSFieldName = "s";
    static constexpr const auto kTFieldName = "t";
  protected:
    const json::Value& value_;

    inline std::optional<TextureWrapMode>
    GetTextureWrapMode(const char* name) const {
      MCC_ASSERT(value_.IsObject());
      if(!value_.HasMember(name))
        return std::nullopt;

      const auto& value = value_[name];
      return ParseTextureWrapMode(std::string(value.GetString(), value.GetStringLength()));
    }
  public:
    explicit JsonTextureWrap(const json::Value& value):
      value_(value) {
    }
    explicit JsonTextureWrap(const json::Document& doc):
      JsonTextureWrap((const json::Value&) doc) {
    }
    virtual ~JsonTextureWrap() = default;

    explicit operator TextureWrap() const {
      TextureWrap wrap;
      if(value_.IsString()) {
        const auto w = std::string(value_.GetString(), value_.GetStringLength()); //normalize w
        const auto& mode = ParseTextureWrapMode(w).value_or(kDefaultWrapMode);
        wrap.r = mode;
        wrap.s = mode;
        wrap.t = mode;
      } else if(value_.IsObject()) {
        wrap.r = GetTextureWrapMode(kRFieldName).value_or(kDefaultWrapMode);
        wrap.s = GetTextureWrapMode(kSFieldName).value_or(kDefaultWrapMode);
        wrap.t = GetTextureWrapMode(kTFieldName).value_or(kDefaultWrapMode);
      }
      
      return wrap;
    }
  };
}

#endif //MCC_TEXTURE_WRAP_H