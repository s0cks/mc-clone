#include <fmt/format.h>
#include <unordered_map>
#include <unordered_set>

#include "mcc/material/material.h"
#include "mcc/texture/texture.h"
#include "mcc/material/material_loader.h"

namespace mcc {
  namespace material {
    static rx::subject<MaterialEvent*> events_;

    rx::observable<MaterialEvent*> OnMaterialEvent() {
      return events_.get_observable();
    }

    Material* Material::LoadFrom(const std::string& filename) {
      return JsonMaterialLoader::LoadMaterial(filename);
    }
  }

  bool RegisterMaterial(const std::string& name) {
    all_materials_.push_back(name);
    return true;
  }

  uint64_t GetNumberOfMaterials() {
    return all_materials_.size();
  }

  bool VisitAllMaterials(std::function<bool(const std::string&)>vis) {
    for(const auto& material : all_materials_) {
      if(!vis(material))
        return false;
    }
    return true;
  }

  static inline std::string
  GetMaterialName(const uri::Uri& uri) {
    auto name = uri.path;
    const auto slashpos = name.find_last_of('/');
    if(slashpos != std::string::npos)
      name = name.substr(slashpos + 1);
    const auto dotpos = name.find_last_of('.');
    if(dotpos != std::string::npos)
      name = name.substr(0, dotpos);
    return name;
  }

  static inline bool
  ForEachMaterialIndexFilename(std::function<bool(const std::string&)> fn) {
    static const std::unordered_set<std::string> kIndexFilenames = {
      "material.json",
      "index.json",
    };
    for(const auto& filename : kIndexFilenames) {
      if(!fn(filename))
        return false;
    }
    return true;
  }

  MaterialRef GetMaterial(const uri::Uri& uri) {
    MCC_ASSERT(uri.scheme == "material");
    const auto name = GetMaterialName(uri);
    DLOG(INFO) << "loading " << name << " from: " << uri;
    const auto root = fmt::format("{}/{}/{}", FLAGS_resources, "materials", uri.path);
    DLOG(INFO) << "root: " << root;

    MaterialRef material;
    ForEachMaterialIndexFilename([&root,&material](const std::string& filename) {
      const auto index = fmt::format("{}/{}", root, filename);
      if(FileExists(index)) {
        DLOG(INFO) << "found: " << index;
        const auto mat = material::JsonMaterialLoader::LoadMaterial(index);
        if(!mat) {
          DLOG(ERROR) << "failed to load material from: " << index;
          return true;// try something else
        }
        
        material = MaterialRef(mat);
        return false; //break loop
      }
      return true;
    });
    if(material.valid())
      return material;

    return MaterialRef();
  }
}