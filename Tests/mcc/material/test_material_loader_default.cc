#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fmt/format.h>

#include "mcc/flags.h"
#include "mcc/material/material_loader.h"

namespace mcc::material {
  class DefaultMaterialLoaderTest : public ::testing::Test {
  protected:
    DefaultMaterialLoaderTest() = default;
  public:
    ~DefaultMaterialLoaderTest() override = default;
  };

  TEST_F(DefaultMaterialLoaderTest, Test_Load_Stones_LunarRock) {
    const auto path = fmt::format("{}/materials/stones/lunar_rock", FLAGS_resources);
    DefaultMaterialLoader loader(path);
    const auto material = loader.Load();
    ASSERT_TRUE(material);
    ASSERT_EQ(material->name(), "stones/lunar_rock");
  }
}