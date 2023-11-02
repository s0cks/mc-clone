#include "mcc/component/test_component_state_table.h"

#include "mcc/gfx.h"

namespace mcc {
  struct TestState {
    glm::vec3 value;
  };

  static constexpr const Entity kE1(1);
  static constexpr const glm::vec3 kE1StateValue = glm::vec3(0.0f, 1.0f, 0.0f);
  static constexpr const TestState kE1State = {
    .value = kE1StateValue,
  };

  static constexpr const Entity kE2(2);
  static constexpr const glm::vec3 kE2StateValue = glm::vec3(1.0f, 0.0f, 1.0f);
  static constexpr const TestState kE2State = {
    .value = kE2StateValue,
  };

  TEST_F(ComponentStateTableTest, TestPut_WillFail_Duplicate) {
    ComponentStateTable<TestState> table;
    ASSERT_TRUE(table.Put(kE1, kE1State));
    ASSERT_FALSE(table.Put(kE1, kE1State));
  }

  TEST_F(ComponentStateTableTest, TestPut_WillPass) {
    ComponentStateTable<TestState> table;
    ASSERT_TRUE(table.Put(kE1, kE1State));
    ASSERT_TRUE(table.Put(kE2, kE2State));
  }

  TEST_F(ComponentStateTableTest, TestGet_WillFail_NoSuchKey) {
    ComponentStateTable<TestState> table;
    ASSERT_TRUE(table.Put(kE1, kE1State));
    ASSERT_TRUE(table.Put(kE2, kE2State));

    static constexpr const Entity kE3(3);
    ASSERT_FALSE(table.Get(kE3));
  }

  TEST_F(ComponentStateTableTest, TestGet_WillPass) {
    ComponentStateTable<TestState> table;
    ASSERT_TRUE(table.Put(kE1, kE1State));
    ASSERT_TRUE(table.Put(kE2, kE2State));

    const auto e1s = table.Get(kE1);
    ASSERT_TRUE(e1s);
    ASSERT_EQ(e1s.value()->value, kE1StateValue);

    const auto e2s = table.Get(kE2);
    ASSERT_TRUE(e2s);
    ASSERT_EQ(e2s.value()->value, kE2StateValue);
  }

  TEST_F(ComponentStateTableTest, TestRemove_WillFail_NoSuchKey) {
    ComponentStateTable<TestState> table;
    ASSERT_TRUE(table.Put(kE1, kE1State));
    ASSERT_TRUE(table.Put(kE2, kE2State));
    
    static constexpr const Entity kE3(3);
    ASSERT_FALSE(table.Remove(kE3));
  }

  TEST_F(ComponentStateTableTest, TestRemove_WillPass) {
    ComponentStateTable<TestState> table;
    ASSERT_TRUE(table.Put(kE1, kE1State));
    ASSERT_TRUE(table.Put(kE2, kE2State));

    {
      const auto e1s = table.Get(kE1);
      ASSERT_TRUE(e1s);
      ASSERT_EQ(e1s.value()->value, kE1StateValue);

      const auto e2s = table.Get(kE2);
      ASSERT_TRUE(e2s);
      ASSERT_EQ(e2s.value()->value, kE2StateValue);
    }

    ASSERT_TRUE(table.Remove(kE1));

    {
      const auto e1s = table.Get(kE1);
      ASSERT_FALSE(e1s);

      const auto e2s = table.Get(kE2);
      ASSERT_TRUE(e2s);
      ASSERT_EQ(e2s.value()->value, kE2StateValue);
    }
  }
}