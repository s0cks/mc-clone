#include "mcc/test_material.h"

namespace mcc {
  TEST_F(MaterialTest, Test_ParsePbrMaterial) {
    const std::string root = FLAGS_resources + "/materials/stones/broken_concrete_1";
    json::Document doc;
    ASSERT_TRUE(json::ParseJson(root + "/material.json", doc));
    material::JsonMaterialLoader loader(root, doc);
    const auto material = loader.LoadMaterial();
    ASSERT_NE(material, nullptr);
    ASSERT_EQ(material->name, "broken_concrete");
    ASSERT_EQ(material->location, root);
  }
}