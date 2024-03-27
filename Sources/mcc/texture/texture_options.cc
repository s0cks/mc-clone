#include "mcc/texture/texture_options.h"

#include <fmt/format.h>
#include <glog/logging.h>

#include "mcc/flags.h"

namespace mcc::texture {
  class DefaultTextureOptionsFactory : public TextureOptionsFactory {
  protected:
    std::string root_;
  public:
    DefaultTextureOptionsFactory():
      root_(fmt::format("{0:s}/textures", FLAGS_resources)) {
    }
    ~DefaultTextureOptionsFactory() override = default;

    bool CreateTextureOptions(const uri::Uri& uri, TextureOptions& results) override {
      const auto ext = uri.GetPathExtension();

      DLOG(ERROR) << "failed to initialize TextureOptions for: " << uri;
      return false;
    }
  };

  static TextureOptionsFactory* factory_ = nullptr;

  TextureOptionsFactory* TextureOptionsFactory::GetInstance() {
    if(factory_ == nullptr)
      factory_ = new DefaultTextureOptionsFactory();
    return factory_;
  }
}