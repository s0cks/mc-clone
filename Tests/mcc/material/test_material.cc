#include <gtest/gtest.h>

#include "mcc/mock_gl.h"
#include "mcc/material/material.h"
#include "mcc/texture/mock_texture.h"

namespace mcc::material {
  class MaterialTest : public ::testing::Test {
  protected:
    MaterialTest() = default;
    ~MaterialTest() override = default;
  };

  TEST_F(MaterialTest, Test_Bind_WillPass) {
    using namespace mcc::texture;

    static constexpr const uint32_t kAlbedoTextureId = 1;
    static constexpr const uint32_t kAoTextureId = 2;
    static constexpr const uint32_t kHeightTextureId = 3;
    static constexpr const uint32_t kMetallicTextureId = 4;
    static constexpr const uint32_t kNormalTextureId = 5;
    static constexpr const uint32_t kRoughnessTextureId = 6;

    MockTexture albedo(kAlbedoTextureId);
    EXPECT_CALL(albedo, Bind(::testing::Eq(Material::kAlbedoTextureSlot)));

    MockTexture ao(kAoTextureId);
    EXPECT_CALL(ao, Bind(::testing::Eq(Material::kAoTextureSlot)));

    MockTexture height(kHeightTextureId);
    EXPECT_CALL(height, Bind(::testing::Eq(Material::kHeightTextureSlot)));

    MockTexture metallic(kMetallicTextureId);
    EXPECT_CALL(metallic, Bind(::testing::Eq(Material::kMetallicTextureSlot)));

    MockTexture normal(kNormalTextureId);
    EXPECT_CALL(normal, Bind(::testing::Eq(Material::kNormalTextureSlot)));

    MockTexture roughness(kRoughnessTextureId);
    EXPECT_CALL(roughness, Bind(::testing::Eq(Material::kRoughnessTextureSlot)));

    Material material("test",
      MockTexture::CreateRef(albedo),
      MockTexture::CreateRef(ao),
      MockTexture::CreateRef(height),
      MockTexture::CreateRef(metallic),
      MockTexture::CreateRef(normal),
      MockTexture::CreateRef(roughness));

    material.Bind();
  }
}