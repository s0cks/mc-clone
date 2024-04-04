#ifndef MCC_TEXTURE_TARGET_H
#define MCC_TEXTURE_TARGET_H

#include "mcc/json.h"
#include "mcc/texture/texture_constants.h"

namespace mcc::texture {
  static inline std::optional<TextureTarget>
  ParseTextureTarget(const std::string& value) {
    if(EqualsIgnoreCase(value, "default")) return { kDefaultTarget };
#define DEFINE_PARSE_TARGET(Name, Target) \
    else if(EqualsIgnoreCase(value, #Name)) { return { k##Name }; }
    FOR_EACH_TEXTURE_TARGET(DEFINE_PARSE_TARGET)
#undef DEFINE_PARSE_TARGET
    DLOG(ERROR) << "unknown texture::TextureTarget: " << value;
    return std::nullopt;
  }

  static inline std::optional<TextureTarget>
  ParseTextureTarget(const json::Value& value) {
    if(!value.IsString())
      return std::nullopt;
    const std::string target(value.GetString(), value.GetStringLength());
    return ParseTextureTarget(target);
  }
}

#endif //MCC_TEXTURE_TARGET_H