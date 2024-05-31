#include "mcc/properties/property_test.h"

namespace mcc::properties {
  class BoolPropertyTest : public PropertyTest {
  protected:
    BoolPropertyTest() = default;
  public:
    ~BoolPropertyTest() override = default;
  };

  TEST_F(BoolPropertyTest, Test_True) {
    static constexpr const auto kPropertyName = "test";
    const auto property = BoolProperty::True(kPropertyName);
    ASSERT_TRUE(property);
    ASSERT_TRUE(IsBoolProperty(property));
    ASSERT_TRUE(HasName(property, kPropertyName));
    ASSERT_TRUE(IsTrue(property));
  }

  TEST_F(PropertyTest, Test_False) {
    static constexpr const auto kPropertyName = "test";
    const auto property = BoolProperty::False(kPropertyName);
    ASSERT_TRUE(property);
    ASSERT_TRUE(IsBoolProperty(property));
    ASSERT_TRUE(HasName(property, kPropertyName));
    ASSERT_TRUE(IsFalse(property));
  }
}