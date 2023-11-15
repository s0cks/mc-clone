#include "mcc/test_material.h"

namespace mcc {
  TEST_F(MaterialTest, Test_Serialization) {    
    MaterialList all_materials;
    GenerateMaterials(all_materials);
    auto buffer = ToBuffer(all_materials);

    MaterialList materials;
    FromBuffer(buffer, materials);

    ASSERT_EQ(materials.size(), all_materials.size());
    for(auto idx = 0; idx < all_materials.size(); idx++) {
      ASSERT_EQ(all_materials[idx], materials[idx]);
    }
  }

  TEST_F(MaterialTest, Test_ParsePbrMaterial) {
    const std::string root = FLAGS_resources + "/materials/stones/broken_concrete_1";
    json::Document doc;
    ASSERT_TRUE(ParseJson(root + "/material.json", doc));
    material::JsonMaterialLoader loader(root, doc);
    const auto material = loader.LoadMaterial();
    ASSERT_NE(material.get(), nullptr);
    ASSERT_EQ(material->name, "broken_concrete");
    ASSERT_EQ(material->location, root);
  }
}