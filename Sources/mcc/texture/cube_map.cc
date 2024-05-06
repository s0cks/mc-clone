#include "mcc/texture/texture.h"

#include "mcc/json_spec.h"
#include "mcc/texture/cube_map_builder.h"
#include "mcc/texture/cube_map_loader_json.h"

namespace mcc::texture {
  CubeMap::CubeMap(const TextureId id):
    TextureTemplate<kCubeMap>(id) {  
  }

  CubeMap::~CubeMap() {
    
  }

  std::string CubeMap::ToString() const {
    std::stringstream ss;
    ss << "CubeMap(";
    ss << "id=" << GetTextureId();
    ss << ")";
    return ss.str();
  }

  static inline CubeMap*
  NewFromDirectory(const std::string& dir) {
    NOT_IMPLEMENTED(FATAL); //TODO: implement
    return nullptr;
  }

  CubeMap* CubeMap::New(const uri::Uri& uri) {
    DLOG(INFO) << "creating CubeMap from " << uri << "....";
    MCC_ASSERT(uri.HasScheme("texture"));
    const auto uri_path = fmt::format("{0:s}/textures/{1:s}", FLAGS_resources, uri.path);
    const auto extension = uri.GetPathExtension();
    if(extension) {
      if(EqualsIgnoreCase(*extension, ".json"))
        return JsonCubeMapLoader::Load(uri_path);
      goto invalid_uri;
    }

    if(IsDirectory(uri_path)) {
      const auto json_path = fmt::format("{0:s}/texture.json", uri_path);
      if(FileExists(json_path))
        return JsonCubeMapLoader::Load(json_path);
      return NewFromDirectory(uri_path);
    }
    
  invalid_uri:
    LOG(ERROR) << "invalid CubeMap uri: " << uri;
    return nullptr;
  }
}