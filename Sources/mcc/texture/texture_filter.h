#ifndef MCC_TEXTURE_FILTER_H
#define MCC_TEXTURE_FILTER_H

#include <string>
#include <optional>

#include "mcc/gfx.h"
#include "mcc/json.h"
#include "mcc/texture/texture_target.h"

namespace mcc::texture {
#define FOR_EACH_TEXTURE_FILTER_COMPONENT(V) \
 V(NearestMipmapNearest, GL_NEAREST_MIPMAP_NEAREST) \
 V(NearestMipmapLinear, GL_NEAREST_MIPMAP_LINEAR) \
 V(LinearMipmapNearest, GL_LINEAR_MIPMAP_NEAREST) \
 V(LinearMipmapLinear, GL_LINEAR_MIPMAP_LINEAR) \
 V(Linear, GL_LINEAR) \
 V(Nearest, GL_NEAREST)

  enum TextureFilterComponent : GLenum {
#define DEFINE_TEXTURE_FILTER_COMPONENT(Name, Value) k##Name = (Value),
    FOR_EACH_TEXTURE_FILTER_COMPONENT(DEFINE_TEXTURE_FILTER_COMPONENT)
#undef DEFINE_TEXTURE_FILTER_COMPONENT

    kDefaultMinFilter = kLinearMipmapLinear,
    kDefaultMagFilter = kLinear,
  };

  static inline std::ostream&
  operator<<(std::ostream& stream, const TextureFilterComponent& rhs) {
    switch(rhs) {
#define DEFINE_TOSTRING(Name, Value) \
      case k##Name: return stream << #Name;
      FOR_EACH_TEXTURE_FILTER_COMPONENT(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
      default:
        return stream << "unknown TextureFilterComponent: " << static_cast<GLenum>(rhs);
    }
  }

  static inline std::optional<TextureFilterComponent>
  ParseTextureFilterComponent(const std::string& value) {
    auto name = value;
    if(name.empty()) {
      return std::nullopt;
    }
    else if(EqualsIgnoreCase(name, "Default")) { return { kLinear }; }
#define DEFINE_PARSE_TEXTURE_FILTER(Name, Value) \
    else if(EqualsIgnoreCase(name, #Name)) { return { k##Name }; }
    FOR_EACH_TEXTURE_FILTER_COMPONENT(DEFINE_PARSE_TEXTURE_FILTER)
#undef DEFINE_PARSE_TEXTURE_FILTER
    DLOG(ERROR) << "unknown TextureFilterComponent: " << name;
    return std::nullopt;
  }

  struct TextureFilter {
    TextureFilterComponent min;
    TextureFilterComponent mag;

    constexpr TextureFilter(const TextureFilterComponent min_filter,
                            const TextureFilterComponent mag_filter):
      min(min_filter),
      mag(mag_filter) {
    }
    constexpr TextureFilter():
      TextureFilter(kDefaultMinFilter, kDefaultMagFilter) {
    }
    explicit constexpr TextureFilter(const TextureFilterComponent component):
      TextureFilter(component, component) {
    }
    TextureFilter(const TextureFilter& rhs) = default;
    ~TextureFilter() = default;

    void ApplyTo(const TextureTarget target) const {
      glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
      CHECK_GL(FATAL);
      glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag);
      CHECK_GL(FATAL);
    }

    friend std::ostream& operator<<(std::ostream& stream, const TextureFilter& rhs) {
      stream << "TextureFilter(";
      stream << "min=" << rhs.min << ", ";
      stream << "mag=" << rhs.mag;
      stream << ")";
      return stream;
    }

    TextureFilter& operator=(const TextureFilter& rhs) = default;

    TextureFilter& operator=(const TextureFilterComponent v) {
      min = mag = v;
      return *this;
    }

    bool operator==(const TextureFilter& rhs) const {
      return min == rhs.min
          && mag == rhs.mag;
    }

    bool operator==(const TextureFilterComponent& rhs) const {
      return min == rhs
          && mag == rhs;
    }

    bool operator!=(const TextureFilter& rhs) const {
      return min != rhs.min
          || mag != rhs.mag;
    }

    bool operator!=(const TextureFilterComponent& rhs) const {
      return min != rhs
          || mag != rhs;
    }
  };

  static constexpr const auto kDefaultFilter = TextureFilter();
  static constexpr const auto kLinearFilter = TextureFilter(kLinear);
  static constexpr const auto kNearestFilter = TextureFilter(kNearest);

  class JsonTextureFilter {
  protected:
    const json::Value& value_;

    inline std::optional<TextureFilterComponent>
    GetFilterComponent(const char* name) const {
      MCC_ASSERT(value_.IsObject());
      if(!value_.HasMember(name))
        return std::nullopt;
      const auto& value = value_[name];
      if(!value.IsString())
        return std::nullopt;
      const auto v = std::string(value.GetString(), value.GetStringLength());
      return ParseTextureFilterComponent(v);
    }
  public:
    explicit JsonTextureFilter(const json::Value& value):
      value_(value) {
    }
    explicit JsonTextureFilter(const json::Document& doc):
      JsonTextureFilter((const json::Value&) doc) {
    }
    virtual ~JsonTextureFilter() = default;
    
    explicit operator TextureFilter() const {
      TextureFilter filter;
      if(value_.IsString()) {
        const auto v = std::string(value_.GetString(), value_.GetStringLength());
        const auto& component = ParseTextureFilterComponent(v).value_or(kLinear); //TODO: fix kLinear
        filter.min = component;
        filter.mag = component;
      } else if(value_.IsObject()) {
        filter.min = GetFilterComponent("min").value_or(kDefaultMinFilter);
        filter.mag = GetFilterComponent("mag").value_or(kDefaultMagFilter);
      }
      return filter;
    }
  };
}

#endif //MCC_TEXTURE_FILTER_H