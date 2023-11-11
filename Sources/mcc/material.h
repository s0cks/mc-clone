#ifndef MCC_MATERIAL_H
#define MCC_MATERIAL_H

#include <fstream>

#include "mcc/gfx.h"
#include "mcc/flags.h"
#include "mcc/buffer.h"
#include "mcc/materials_generated.h"

namespace mcc {
  static constexpr const auto kDefaultMaterialsFilename = "";
  DECLARE_string(materials_file);

  static inline std::string
  GetMaterialsFilename() {
    if(!FLAGS_materials_file.empty())
      return FLAGS_materials_file;
    return FLAGS_resources + "/materials.dat";
  }

  struct Material {
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    friend std::ostream& operator<<(std::ostream& stream, const Material& rhs) {
      stream << "Material(";
      stream << "name=" << rhs.name << ", ";
      stream << "ambient=" << glm::to_string(rhs.ambient) << ", ";
      stream << "diffuse=" << glm::to_string(rhs.diffuse) << ", ";
      stream << "specular=" << glm::to_string(rhs.specular) << ", ";
      stream << "shininess=" << rhs.shininess;
      stream << ")";
      return stream;
    }

    bool operator==(const Material& rhs) const {
      return name == rhs.name
          && ambient == rhs.ambient
          && diffuse == rhs.diffuse
          && specular == rhs.specular
          && shininess == rhs.shininess;
    }
  };

  typedef std::vector<Material> MaterialList;
  typedef std::unordered_map<std::string, Material> MaterialMap;

  static void
  Create(flatbuffers::FlatBufferBuilder& builder,
         const glm::vec3& ambient,
         const glm::vec3& diffuse,
         const glm::vec3& specular,
         const float& shininess) {
    const fb::Vec3 a(ambient[0], ambient[1], ambient[2]);
    const fb::Vec3 d(diffuse[0], diffuse[1], diffuse[2]);
    const fb::Vec3 s(specular[0], specular[1], specular[2]);
    fb::MaterialBuilder material(builder);
    material.add_ambient(&a);
    material.add_diffuse(&d);
    material.add_specular(&s);
    material.add_shininess(shininess);
    const auto mat = material.Finish();
    builder.Finish(mat);
  }

  void GenerateMaterials(MaterialList& materials);
  void InitializeMaterials(std::string filename = GetMaterialsFilename());
  BufferPtr ToBuffer(const MaterialList& materials);
  void FromBuffer(BufferPtr data, MaterialList& materials);
  void FromBuffer(BufferPtr data, MaterialMap& materials);

  Material GetDefaultMaterial();
  Material GetMaterial(const std::string& name);
}

#endif //MCC_MATERIAL_H