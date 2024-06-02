#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mcc/shader/shader_set.h"
#include "mcc/shader/mock_shader.h"

namespace mcc {
  using namespace ::testing;
  using shader::MockShader;

  class ShaderSetTest : public Test {
  protected:
    ShaderSetTest() = default;
  public:
    ~ShaderSetTest() override = default;
  };

  TEST_F(ShaderSetTest, Test_Insert_WillFail_NullShaderPointer) {
    ShaderSet shaders;
    ASSERT_FALSE(shaders.Insert(nullptr));
  }

  TEST_F(ShaderSetTest, Test_Insert1_WillPass) {
    static constexpr const ShaderId kShaderId1 = 100;
    MockShader a(Metadata(), kShaderId1, shader::kVertexShader);
    ShaderSet shaders;
    ASSERT_TRUE(shaders.Insert(&a));
  }

  TEST_F(ShaderSetTest, Test_Insert2_WillPass) {
    static constexpr const ShaderId kShaderId1 = 100;
    static constexpr const ShaderId kShaderId2 = 110;
    MockShader a(Metadata(), kShaderId1, shader::kVertexShader);
    MockShader b(Metadata(), kShaderId2, shader::kFragmentShader);
    ShaderSet shaders;
    ASSERT_TRUE(shaders.Insert(&a));
    ASSERT_TRUE(shaders.Insert(&b));
  }

  TEST_F(ShaderSetTest, Test_Contains_WillFail_DoesntExist) {
    static constexpr const ShaderId kShaderId1 = 100;
    MockShader a(Metadata(), kShaderId1, shader::kVertexShader);
    ShaderSet shaders;
    ASSERT_FALSE(shaders.Contains(&a));
  }

  TEST_F(ShaderSetTest, Test_Contains1_WillFail_DoesntExist) {
    static constexpr const ShaderId kShaderId1 = 100;
    static constexpr const ShaderId kShaderId2 = 110;
    MockShader a(Metadata(), kShaderId1, shader::kVertexShader);
    MockShader b(Metadata(), kShaderId2, shader::kFragmentShader);
    ShaderSet shaders;
    ASSERT_TRUE(shaders.Insert(&a));
    ASSERT_TRUE(shaders.Contains(&a));
    ASSERT_FALSE(shaders.Contains(&b));
  }
}