#include "mcc/test_resource.h"

namespace mcc {
  TEST_F(ResourceTest, Test_Constructor0) {
    Resource none;
    ASSERT_EQ(none.type(), Resource::kNone);
    ASSERT_EQ(none.hash(), 0);
    ASSERT_TRUE(none.value().empty());
  }

  TEST_F(ResourceTest, Test_Constructor1) {
    Resource k1(Resource::kShader, "test");
    ASSERT_EQ(k1.type(), Resource::kShader);
    ASSERT_EQ(k1.hash(), Murmur("test"));
    ASSERT_EQ(k1.value(), "test");
  }

  TEST_F(ResourceTest, Test_Equals) {
    Resource k1(Resource::kShader, "test");
    ASSERT_EQ(k1, k1);
  }

  TEST_F(ResourceTest, Test_NotEquals) {
    Resource k1(Resource::kShader, "test");
    Resource k2(Resource::kShader, "test2");
    ASSERT_EQ(k1, k1);
    ASSERT_EQ(k2, k2);
    ASSERT_NE(k1, k2);
    ASSERT_NE(k2, k1);
  }

  TEST_F(ResourceTest, Test_LessThan_TypeNotEqual) {
    Resource k1(Resource::kShader, "test");
    Resource k2(Resource::kMaterial, "test");
    ASSERT_LT(k1, k2);
    ASSERT_FALSE(k1 < k1);
    ASSERT_FALSE(k2 < k2);
  }

  TEST_F(ResourceTest, Test_LessThan_KeyNotEqual) {
    Resource k1(Resource::kShader, "test1");
    Resource k2(Resource::kShader, "test2");
    ASSERT_LT(k1, k2);
    ASSERT_FALSE(k1 < k1);
    ASSERT_FALSE(k2 < k2);
  }

  TEST_F(ResourceTest, Test_GreaterThan_TypeNotEqual) {
    Resource k1(Resource::kMaterial, "test");
    Resource k2(Resource::kShader, "test");
    ASSERT_GT(k1, k2);
    ASSERT_FALSE(k1 > k1);
    ASSERT_FALSE(k2 > k2);
  }

  TEST_F(ResourceTest, Test_GreaterThan_KeyNotEqual) {
    Resource k1(Resource::kShader, "test2");
    Resource k2(Resource::kShader, "test1");
    ASSERT_GT(k1, k2);
    ASSERT_FALSE(k1 > k1);
    ASSERT_FALSE(k2 > k2);
  }
}