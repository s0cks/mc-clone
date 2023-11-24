#include "mcc/texture/texture.h"
#include <regex>

#include "mcc/uri.h"
#include "mcc/flags.h"
#include "mcc/texture/image.h"
#include "mcc/texture/texture_loader.h"

namespace mcc {
  namespace texture {
    static constexpr const auto kProtocolPattern = "^tex(?:ture)?:(?:\\/\\/)?.*$";
    static constexpr const auto kProtocolName = "texture";
    class TextureUriProtocol : public uri::Protocol {
    private:
      std::regex pattern_;
    public:
      TextureUriProtocol():
        uri::Protocol(),
        pattern_(kProtocolPattern) {
      }
      ~TextureUriProtocol() override = default;

      std::string GetName() const override {
        return kProtocolName;
      }
      
      bool Matches(const std::string& protocol) const override {
        return std::regex_match(protocol, pattern_);
      }

      std::string Resolve(const uri::Uri& uri) const override {
        return FLAGS_resources + "/textures/" + uri.location;
      }
    };

    void Texture::Init() {
      uri::RegisterProtocol<TextureUriProtocol>();
    }
  }

  static const std::regex kPngPattern(".*\\.(png)$");
  static const std::regex kJpegPattern(".*\\.(jpeg|jpg)$");
  static const std::regex kJsonPattern(".*\\.(json)$");

  TextureRef GetTexture(const resource::Token& token) {
    const auto& tag = token.tag;
    MCC_ASSERT(tag.type() == resource::kTextureType);
    const auto& filename = token.location;
    texture::TextureLoader loader(tag, filename);
    return loader.Load();
  }

  TextureRef GetTexture(const uri::Uri& uri) {
    MCC_ASSERT(uri.protocol == std::string(texture::kProtocolName));
    texture::TextureLoader loader(res::NewTextureTag(uri.location), uri.location);
    return loader.Load();
  }
}