#include "mcc/material.h"

namespace mcc {
  DEFINE_string(materials_file, kDefaultMaterialsFilename, "The name of the materials.dat file");

  void GenerateMaterials(MaterialList& materials) {
    materials.push_back({
      .name = "emerald",
      .ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f),
      .diffuse = glm::vec3(0.07568f, 0.61424f, 0.07568f),
      .specular = glm::vec3(0.633f, 0.727811f, 0.633f),
      .shininess = 0.6f,
    });
    materials.push_back({
      .name = "jade",
      .ambient = glm::vec3(0.135f, 0.2225f, 0.1575f),
      .diffuse = glm::vec3(0.54f, 0.89f, 0.63f),
      .specular = glm::vec3(0.316228f, 0.316228f, 0.316228f),
      .shininess = 0.1f,
    });
    materials.push_back({
      .name = "obsidian",
      .ambient = glm::vec3(0.05375f, 0.05f, 0.06625f),
      .diffuse = glm::vec3(0.18275f, 0.17f, 0.22525f),
      .specular = glm::vec3(0.332741f, 0.328634f, 0.346435f),
      .shininess = 0.3f,
    });
  }

  static MaterialMap all_materials;

  void InitializeMaterials(std::string filename) {
    if(!FileExists(filename)) {
      DLOG(INFO) << "materials file " << filename << " doesn't exist, generating....";
      MaterialList materials;
      GenerateMaterials(materials);
      const auto buffer = ToBuffer(materials);
      std::fstream stream(filename, std::ios::out|std::ios::binary);
      buffer->WriteTo(stream);
      stream.flush();
      stream.close();
    } else {
      const auto buffer = Buffer::FromFile(filename);
      FromBuffer(buffer, all_materials);
    }
  }

  BufferPtr ToBuffer(const MaterialList& materials) {
    flatbuffers::FlatBufferBuilder builder(1024);

    std::vector<flatbuffers::Offset<fb::Material>> offsets;
    for(const auto& material : materials) {
      const auto name = builder.CreateString(material.name);
      fb::Vec3 ambient(material.ambient[0], material.ambient[1], material.ambient[2]);
      fb::Vec3 diffuse(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
      fb::Vec3 specular(material.specular[0], material.specular[1], material.specular[2]);
      const auto mat = fb::CreateMaterial(builder, name, &ambient, &diffuse, &specular, material.shininess);
      offsets.push_back(mat);
    }
    const auto mats = fb::CreateMaterials(builder, builder.CreateVector(offsets));
    builder.Finish(mats);

    const auto data = builder.GetBufferPointer();
    const auto size = builder.GetSize();

    auto buffer = Buffer::New(size);
    LOG_IF(FATAL, !buffer->Put(data, size)) << "failed to put material data into buffer.";
    return buffer;
  }

  void FromBuffer(BufferPtr data, MaterialList& materials) {
    const auto mats = fb::GetMaterials(data->data())->materials();
    for(auto mat : (*mats)) {
      materials.push_back(Material {
        .name = std::string(mat->name()->data(), mat->name()->size()),
        .ambient = glm::vec3(mat->ambient()->x(), mat->ambient()->y(), mat->ambient()->z()),
        .diffuse = glm::vec3(mat->diffuse()->x(), mat->diffuse()->y(), mat->diffuse()->z()),
        .specular = glm::vec3(mat->specular()->x(), mat->specular()->y(), mat->specular()->z()),
        .shininess = mat->shininess(),
      });
    }
  }

  void FromBuffer(BufferPtr data, MaterialMap& materials) {
    const auto mats = fb::GetMaterials(data->data());
    for(auto mat : (*mats->materials())) {
      const auto name = std::string(mat->name()->data(), mat->name()->size());
      materials.insert({
        name,
        Material {
          .name = name,
          .ambient = glm::vec3(mat->ambient()->x(), mat->ambient()->y(), mat->ambient()->z()),
          .diffuse = glm::vec3(mat->diffuse()->x(), mat->diffuse()->y(), mat->diffuse()->z()),
          .specular = glm::vec3(mat->specular()->x(), mat->specular()->y(), mat->specular()->z()),
          .shininess = mat->shininess(),
        }
      });
    }
  }

  static Material kDefaultMaterial = {
    .name = "default",
  };

  Material GetDefaultMaterial() {
    return kDefaultMaterial;
  }

  Material GetMaterial(const std::string& name) {
    const auto pos = all_materials.find(name);
    return pos == all_materials.end()
         ? kDefaultMaterial
         : pos->second;
  }
}