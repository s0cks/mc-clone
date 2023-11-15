#include "mcc/test_resource.h"

namespace mcc {
  using resource::Tag;

  TEST_F(ResourceTest, Test_Constructor0) {
    Tag none;
    ASSERT_EQ(none.type(), resource::kUnknownType);
    ASSERT_EQ(none.hash(), 0);
    ASSERT_TRUE(none.value().empty());
  }

  TEST_F(ResourceTest, Test_Constructor1) {
    Tag k1(resource::kShaderType, "test");
    ASSERT_EQ(k1.type(), resource::kShaderType);
    ASSERT_EQ(k1.hash(), Murmur("test"));
    ASSERT_EQ(k1.value(), "test");
  }

  TEST_F(ResourceTest, Test_Equals) {
    Tag k1(resource::kShaderType, "test");
    ASSERT_EQ(k1, k1);
  }

  TEST_F(ResourceTest, Test_NotEquals) {
    Tag k1(resource::kShaderType, "test");
    Tag k2(resource::kShaderType, "test2");
    ASSERT_EQ(k1, k1);
    ASSERT_EQ(k2, k2);
    ASSERT_NE(k1, k2);
    ASSERT_NE(k2, k1);
  }

  TEST_F(ResourceTest, Test_LessThan_TypeNotEqual) {
    Tag k1(resource::kShaderType, "test");
    Tag k2(resource::kMaterialType, "test");
    ASSERT_LT(k1, k2);
    ASSERT_FALSE(k1 < k1);
    ASSERT_FALSE(k2 < k2);
  }

  TEST_F(ResourceTest, Test_LessThan_KeyNotEqual) {
    Tag k1(resource::kShaderType, "test1");
    Tag k2(resource::kShaderType, "test2");
    ASSERT_LT(k1, k2);
    ASSERT_FALSE(k1 < k1);
    ASSERT_FALSE(k2 < k2);
  }

  TEST_F(ResourceTest, Test_GreaterThan_TypeNotEqual) {
    Tag k1(resource::kMaterialType, "test");
    Tag k2(resource::kShaderType, "test");
    ASSERT_GT(k1, k2);
    ASSERT_FALSE(k1 > k1);
    ASSERT_FALSE(k2 > k2);
  }

  TEST_F(ResourceTest, Test_GreaterThan_KeyNotEqual) {
    Tag k1(resource::kShaderType, "test2");
    Tag k2(resource::kShaderType, "test1");
    ASSERT_GT(k1, k2);
    ASSERT_FALSE(k1 > k1);
    ASSERT_FALSE(k2 > k2);
  }
}