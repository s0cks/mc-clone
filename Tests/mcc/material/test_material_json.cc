#include <gtest/gtest.h>
#include <fmt/format.h>

#include "mcc/flags.h"
#include "mcc/material/material_json.h"

namespace mcc::material {
  class MaterialDocumentTest : public ::testing::Test {
  protected:
    MaterialDocumentTest() = default;
  public:
    ~MaterialDocumentTest() override = default;
  };

  static inline std::string
  GetMaterialDoc(const std::string& name) {
    return fmt::format("{0:s}/materials/{1:s}/material.json", FLAGS_resources, name);
  }

  static inline std::string
  GetMaterialTexturePath(const std::string& material, const std::string& texture) {
    return fmt::format("{0:s}/materials/{1:s}/{2:s}", FLAGS_resources, material, texture);
  }

  TEST_F(MaterialDocumentTest, Test_FileNotFound) {
    const auto filename = GetMaterialDoc("missing.json");
    MaterialDocument doc(filename);
    ASSERT_FALSE(doc.IsValid());
  }

  TEST_F(MaterialDocumentTest, Test_Empty) {
    const auto filename = GetMaterialDoc("empty.json");
    MaterialDocument doc(filename);
    ASSERT_FALSE(doc.IsValid());
  }

  TEST_F(MaterialDocumentTest, Test_InvalidSchema0) {
    const auto filename = GetMaterialDoc("invalid-schema-0");
    MaterialDocument doc(filename);
    ASSERT_FALSE(doc.IsValid());
  }

  TEST_F(MaterialDocumentTest, Test_ValidSchema0) {
    const auto filename = GetMaterialDoc("valid-schema-0");
    MaterialDocument doc(filename);
    ASSERT_TRUE(doc.IsValid());

    const auto albedo = doc.GetAlbedoProperty();
    ASSERT_TRUE(albedo);
    ASSERT_EQ((*albedo), "test");


    const auto ao = doc.GetAoProperty();
    ASSERT_TRUE(ao);
    ASSERT_EQ((*ao), "ambient");

    const auto metallic = doc.GetMetallicProperty();
    ASSERT_FALSE(metallic);
  }
}