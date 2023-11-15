#ifndef MCC_MATERIAL_H
#define MCC_MATERIAL_H

#include <fstream>
#include <memory>

#include "mcc/gfx.h"
#include "mcc/flags.h"
#include "mcc/buffer.h"
#include "mcc/materials_generated.h"

#include "mcc/json.h"
#include "mcc/texture/texture.h"

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

  namespace material {
    struct Material;
    typedef std::shared_ptr<Material> MaterialPtr;
    struct Material {
      std::string name;
      std::string location;
      Texture albedo;
      Texture ao;
      Texture height;
      Texture metallic;
      Texture normal;
      Texture roughness;

      void Bind() const {
        albedo.Bind(0);
        ao.Bind(1);
        height.Bind(2);
        metallic.Bind(3);
        normal.Bind(4);
        roughness.Bind(5);
      }
    public:
      static MaterialPtr LoadFrom(const std::string& filename);
    };

    class MaterialLoader {
    protected:
      MaterialLoader() = default;
    public:
      virtual ~MaterialLoader() = default;
      virtual MaterialPtr LoadMaterial() = 0;
    };

    class JsonMaterialLoader : public MaterialLoader {
    private:
      std::string root_;
      json::Document& doc_;

      inline Texture ParseMaterialComponent(const char* name) {
        if(!doc_.HasMember(name))
          return kInvalidTextureId;
        const auto& value = doc_[name];
        if(value.IsBool()) {
          if(!value.GetBool()) // not enabled
            return kInvalidTextureId;
          const auto filename = root_ + "/" + name + ".png";
          DLOG(INFO) << "loading material " << name << " texture from: " << filename;
          return Texture::LoadFrom(filename);
        } else if(value.IsString()) {
          const auto filename = root_ + "/" + value.GetString() + ".png";
          DLOG(INFO) << "loading material " << name << " texture from: " << filename;
          return Texture::LoadFrom(filename);
        }
        DLOG(INFO) << "cannot determine material component '" << name << "' from json value";
        return kInvalidTextureId;
      }
    public:
      explicit JsonMaterialLoader(const std::string& root,
                                  json::Document& doc):
        MaterialLoader(),
        root_(root),
        doc_(doc) {
      }
      ~JsonMaterialLoader() override = default;
      MaterialPtr LoadMaterial() override;
    public:
      static inline MaterialPtr
      Load(const std::string& root) {
        json::Document doc;
        if(!ParseJson(root + "/material.json", doc))
          return nullptr;
        material::JsonMaterialLoader loader(root, doc);
        return loader.LoadMaterial();
      }
    };
  }
}

#endif //MCC_MATERIAL_H