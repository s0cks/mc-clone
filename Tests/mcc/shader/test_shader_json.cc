#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <glog/logging.h>

#include "mcc/shader/shader_json.h"

namespace mcc {
  using namespace shader;

  class ShaderTest : public ::testing::Test {
  protected:
    ShaderTest() = default;
  public:
    ~ShaderTest() override = default;
  };

  class ShaderSchemaTest : public ::testing::Test {
  protected:
    ShaderSchemaTest() = default;
  public:
    ~ShaderSchemaTest() override = default;
  };

  TEST_F(ShaderSchemaTest, Test_EmptyDocument) {
    const auto filename = FLAGS_resources + "/shaders/empty.json";
    ShaderDocument doc(filename);
    ASSERT_FALSE(doc.IsValid());
  }

  TEST_F(ShaderSchemaTest, Test_MissingDocument) {
    const auto filename = FLAGS_resources + "/shaders/missing.json";
    ShaderDocument doc(filename);
    ASSERT_FALSE(doc.IsValid());
  }

  TEST_F(ShaderSchemaTest, Test_InvalidSchema0) {
    const auto filename = FLAGS_resources + "/shaders/invalid-shader-schema0.json";
    ShaderDocument doc(filename);
    ASSERT_FALSE(doc.IsValid());
  }

  TEST_F(ShaderSchemaTest, Test_InvalidSchema1) {
    const auto filename = FLAGS_resources + "/shaders/invalid-shader-schema1.json";
    ShaderDocument doc(filename);
    ASSERT_FALSE(doc.IsValid());
  }

  TEST_F(ShaderSchemaTest, Test_InvalidSchema2) {
    const auto filename = FLAGS_resources + "/shaders/invalid-shader-schema2.json";
    ShaderDocument doc(filename);
    ASSERT_FALSE(doc.IsValid());
  }

  TEST_F(ShaderSchemaTest, Test_InvalidSchema3) {
    const auto filename = FLAGS_resources + "/shaders/invalid-shader-schema3.json";
    ShaderDocument doc(filename);
    ASSERT_FALSE(doc.IsValid());
  }

  TEST_F(ShaderSchemaTest, Test_ValidSchema0) {
    const auto filename = FLAGS_resources + "/shaders/valid-shader-schema0.json";
    ShaderDocument doc(filename);
    ASSERT_TRUE(doc.IsValid());
    ASSERT_EQ(doc.GetVertexShader(), FLAGS_resources + "/shaders/valid-shader-schema0.vs");
    ASSERT_EQ(doc.GetFragmentShader(), FLAGS_resources + "/shaders/valid-shader-schema0.fs");
    const auto geometryShader = doc.GetGeometryShader();
    ASSERT_FALSE(geometryShader);
    const auto tessEvalShader = doc.GetTessEvalShader();
    ASSERT_FALSE(tessEvalShader);
    const auto tessControlShader = doc.GetTessControlShader();
    ASSERT_FALSE(tessControlShader);
  }

  TEST_F(ShaderSchemaTest, Test_ValidSchema1) {
    const auto filename = FLAGS_resources + "/shaders/valid-shader-schema1.json";
    ShaderDocument doc(filename);
    ASSERT_TRUE(doc.IsValid());
    ASSERT_EQ(doc.GetVertexShader(), FLAGS_resources + "/shaders/valid-shader-schema1.vs");
    ASSERT_EQ(doc.GetFragmentShader(), FLAGS_resources + "/shaders/valid-shader-schema1.fs");
    const auto geometryShader = doc.GetGeometryShader();
    ASSERT_TRUE(geometryShader);
    ASSERT_EQ((*geometryShader), FLAGS_resources + "/shaders/valid-shader-schema1.gs");
    const auto tessEvalShader = doc.GetTessEvalShader();
    ASSERT_FALSE(tessEvalShader);
    const auto tessControlShader = doc.GetTessControlShader();
    ASSERT_FALSE(tessControlShader);
  }

  TEST_F(ShaderSchemaTest, Test_ValidSchema2) {
    const auto filename = FLAGS_resources + "/shaders/valid-shader-schema2.json";
    ShaderDocument doc(filename);
    ASSERT_TRUE(doc.IsValid());
    ASSERT_EQ(doc.GetVertexShader(), FLAGS_resources + "/shaders/valid-shader-schema2.vs");
    ASSERT_EQ(doc.GetFragmentShader(), FLAGS_resources + "/shaders/valid-shader-schema2.fs");
    const auto geometryShader = doc.GetGeometryShader();
    ASSERT_TRUE(geometryShader);
    ASSERT_EQ((*geometryShader), FLAGS_resources + "/shaders/valid-shader-schema2.gs");
    const auto tessEvalShader = doc.GetTessEvalShader();
    ASSERT_TRUE(tessEvalShader);
    ASSERT_EQ((*tessEvalShader), FLAGS_resources + "/shaders/valid-shader-schema2.tes");
    const auto tessControlShader = doc.GetTessControlShader();
    ASSERT_FALSE(tessControlShader);
  }

  TEST_F(ShaderSchemaTest, Test_ValidSchema3) {
    const auto filename = FLAGS_resources + "/shaders/valid-shader-schema3.json";
    ShaderDocument doc(filename);
    ASSERT_TRUE(doc.IsValid());
    ASSERT_EQ(doc.GetVertexShader(), FLAGS_resources + "/shaders/valid-shader-schema3.vs");
    ASSERT_EQ(doc.GetFragmentShader(), FLAGS_resources + "/shaders/valid-shader-schema3.fs");
    const auto geometryShader = doc.GetGeometryShader();
    ASSERT_TRUE(geometryShader);
    ASSERT_EQ((*geometryShader), FLAGS_resources + "/shaders/valid-shader-schema3.gs");
    const auto tessEvalShader = doc.GetTessEvalShader();
    ASSERT_TRUE(tessEvalShader);
    ASSERT_EQ((*tessEvalShader), FLAGS_resources + "/shaders/valid-shader-schema3.tes");
    const auto tessControlShader = doc.GetTessControlShader();
    ASSERT_TRUE(tessControlShader);
    ASSERT_EQ((*tessControlShader), FLAGS_resources + "/shaders/valid-shader-schema3.tcs");
  }

  TEST_F(ShaderSchemaTest, Test_ValidSchema4) {
    const auto filename = FLAGS_resources + "/shaders/valid-shader-schema4.json";
    ShaderDocument doc(filename);
    ASSERT_TRUE(doc.IsValid());
  }

  TEST_F(ShaderSchemaTest, Test_ValidSchema5) {
    const auto filename = FLAGS_resources + "/shaders/valid-shader-schema5.json";
    ShaderDocument doc(filename);
    ASSERT_TRUE(doc.IsValid());
  }

  TEST_F(ShaderSchemaTest, Test_ValidSchema6) {
    const auto filename = FLAGS_resources + "/shaders/valid-shader-schema6.json";
    ShaderDocument doc(filename);
    ASSERT_TRUE(doc.IsValid());
  }

  TEST_F(ShaderSchemaTest, Test_ValidSchema7) {
    const auto filename = FLAGS_resources + "/shaders/valid-shader-schema7.json";
    ShaderDocument doc(filename);
    ASSERT_TRUE(doc.IsValid());
  }
}