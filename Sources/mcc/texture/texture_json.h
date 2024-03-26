#ifndef MCC_TEXTURE_JSON_H
#define MCC_TEXTURE_JSON_H

#include <optional>

#include "mcc/json.h"
#include "mcc/texture/texture.h"

namespace mcc::texture {
  class JsonTexture {
    DEFINE_NON_COPYABLE_TYPE(JsonTexture);
    static constexpr const auto kFilenameFieldName = "filename";
    static constexpr const auto kTargetFieldName = "target";
    static constexpr const auto kFilterFieldName = "filter";
    static constexpr const auto kWrapFieldName = "wrap";
    static constexpr const auto kAlignmentFieldName = "alignment";
  protected:
    const json::Value& value_;

    inline std::optional<std::string>
    GetStringProperty(const char* name) const {
      MCC_ASSERT(IsObject());
      const auto& value = value_[name];
      if(!value.IsString())
        return std::nullopt;
      return { std::string(value.GetString(), value.GetStringLength()) };
    }

    inline bool
    HasProperty(const char* name) const {
      MCC_ASSERT(IsObject());
      return value_.HasMember(name);
    }
  public:
    JsonTexture() = delete;
    explicit JsonTexture(const json::Value& value):
      value_(value) {
    }
    explicit JsonTexture(const json::Document& doc):
      JsonTexture((const json::Value&) doc) {
    }
    virtual ~JsonTexture() = default;

    const json::Value& value() const {
      return value_;
    }

    inline bool IsObject() const {
      return value().IsObject();
    }

    inline bool IsString() const {
      return value().IsString();
    }

    std::optional<TextureTarget> GetTarget() const {
      if(!IsObject())
        return std::nullopt;
      if(!HasProperty(kTargetFieldName))
        return std::nullopt;
      return ParseTextureTarget((const json::Value&) value_[kTargetFieldName]);
    }

    std::optional<TextureFilter> GetFilter() const {
      if(!IsObject())
        return std::nullopt;
      if(!HasProperty(kFilterFieldName))
        return std::nullopt;
      JsonTextureFilter filter((const json::Value&) value_[kFilterFieldName]);
      return { (const TextureFilter) filter };
    }

    std::optional<TextureWrap> GetWrap() const {
      if(!IsObject())
        return std::nullopt;
      if(!HasProperty(kWrapFieldName))
        return std::nullopt;
      JsonTextureWrap wrap((const json::Value&) value_[kWrapFieldName]);
      return { (const TextureWrap) wrap };
    }

    std::optional<std::string> GetFilename() const {
      if(!IsObject())
        return std::nullopt;
      if(!HasProperty(kFilenameFieldName))
        return std::nullopt;
      return GetStringProperty(kFilenameFieldName);
    }

    bool IsValid() const;
  };
}

#endif //MCC_TEXTURE_JSON_H