#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mcc/mock_gl.h"
#include "mcc/texture/texture_wrap.h"

namespace mcc::texture {
  class TextureWrapTest : public ::testing::Test {
  protected:
    TextureWrapTest() = default;
  public:
    ~TextureWrapTest() override = default;
  };

  TEST_F(TextureWrapTest, Test_ClampToBorder_ApplyTo_Texture2D) {
    static const auto target = k2D;
    static const auto wrap = kClampToBorderWrap;

    MockGl gl;
    EXPECT_CALL(gl, glTexParameteri(::testing::Eq(target), ::testing::Eq(GL_TEXTURE_WRAP_R), ::testing::Eq(wrap.r)));
    EXPECT_CALL(gl, glTexParameteri(::testing::Eq(target), ::testing::Eq(GL_TEXTURE_WRAP_S), ::testing::Eq(wrap.s)));
    EXPECT_CALL(gl, glTexParameteri(::testing::Eq(target), ::testing::Eq(GL_TEXTURE_WRAP_T), ::testing::Eq(wrap.t)));
    wrap.ApplyTo(target);
  }

  TEST_F(TextureWrapTest, Test_ParseTextureWrap_WillFail_EmptyValue) {
    const auto mode = ParseTextureWrapMode("");
    ASSERT_FALSE(mode);
  }

  TEST_F(TextureWrapTest, Test_ParseTextureWrap_WillFail_UnknownValue) {
    const auto mode = ParseTextureWrapMode("thisisaninvalidtexturewrapmode");
    ASSERT_FALSE(mode);
  }
  
  TEST_F(TextureWrapTest, Test_ParseTextureWrap_WillPass_Default) {
    const auto mode = ParseTextureWrapMode("default");
    ASSERT_TRUE(mode);
    ASSERT_EQ((*mode), kDefaultWrapMode);
  }

  TEST_F(TextureWrapTest, Test_ParseTextureWrap_WillPass_Repeat) {
    const auto mode = ParseTextureWrapMode("repeat");
    ASSERT_TRUE(mode);
    ASSERT_EQ((*mode), kRepeat);
  }

  TEST_F(TextureWrapTest, Test_ParseTextureWrap_WillPass_ClampToEdge) {
    const auto mode = ParseTextureWrapMode("clamp2edge");
    ASSERT_TRUE(mode);
    ASSERT_EQ((*mode), kClampToEdge);
  }

  TEST_F(TextureWrapTest, Test_ParseTextureWrap_WillPass_ClampToBorder) {
    const auto mode = ParseTextureWrapMode("clamp2border");
    ASSERT_TRUE(mode);
    ASSERT_EQ((*mode), kClampToBorder);
  }

  class JsonTextureWrapParserTest : public ::testing::Test {
  protected:
    JsonTextureWrapParserTest() = default;
  public:
    ~JsonTextureWrapParserTest() override = default;
  };

  static inline ::testing::AssertionResult
  IsTextureWrap(const TextureWrap& wrap, const TextureWrapMode mode) {
    if(wrap.r != mode)
      return ::testing::AssertionFailure() << "expected " << wrap << ".r to be: " << mode;
    if(wrap.s != mode)
      return ::testing::AssertionFailure() << "expected " << wrap << ".s to be: " << mode;
    if(wrap.t != mode)
      return ::testing::AssertionFailure() << "expected " << wrap << ".t to be: " << mode;
    return ::testing::AssertionSuccess();
  }

  static inline ::testing::AssertionResult
  IsTextureWrap(const TextureWrap& wrap, const TextureWrapMode r, const TextureWrapMode s, const TextureWrapMode t) {
    if(wrap.r != r)
      return ::testing::AssertionFailure() << "expected " << wrap << ".r to be: " << r;
    if(wrap.s != s)
      return ::testing::AssertionFailure() << "expected " << wrap << ".s to be: " << s;
    if(wrap.t != t)
      return ::testing::AssertionFailure() << "expected " << wrap << ".t to be: " << t;
    return ::testing::AssertionSuccess();
  }

  static inline ::testing::AssertionResult
  IsTextureWrap(const TextureWrap& wrap, const TextureWrap& expected) {
    if(wrap != expected)
      return ::testing::AssertionFailure() << "expected " << wrap << " to be: " << expected;
    return ::testing::AssertionSuccess();
  }

  TEST_F(JsonTextureWrapParserTest, Test_Parse_WillPass_EmptyDocument) {
    json::Document doc;
    json::ParseJson(doc, "{}");

    const json::Value& value = doc.GetObject();
    JsonTextureWrapParser parser(value);

    TextureWrap wrap;
    ASSERT_TRUE(parser.Parse(wrap));
    ASSERT_TRUE(IsTextureWrap(wrap, kDefaultWrapMode));
  }

  TEST_F(JsonTextureWrapParserTest, Test_Parse_WillPass_REqualsRepeat) {
    json::Document doc;
    json::ParseJson(doc, "{ \"r\": \"repeat\" }");
    const json::Value& value = doc.GetObject();
    JsonTextureWrapParser parser(value);

    TextureWrap wrap;
    ASSERT_TRUE(parser.Parse(wrap));
    ASSERT_TRUE(IsTextureWrap(wrap, kRepeat, kDefaultWrapMode, kDefaultWrapMode));
  }

  TEST_F(JsonTextureWrapParserTest, Test_Parse_WillPass_SEqualsRepeat) {
    json::Document doc;
    json::ParseJson(doc, "{ \"s\": \"repeat\" }");
    const json::Value& value = doc.GetObject();
    JsonTextureWrapParser parser(value);

    TextureWrap wrap;
    ASSERT_TRUE(parser.Parse(wrap));
    ASSERT_TRUE(IsTextureWrap(wrap, kDefaultWrapMode, kRepeat, kDefaultWrapMode));
  }

  TEST_F(JsonTextureWrapParserTest, Test_Parse_WillPass_TEqualsRepeat) {
    json::Document doc;
    json::ParseJson(doc, "{ \"t\": \"repeat\" }");
    JsonTextureWrapParser parser((const json::Value&) doc);

    TextureWrap wrap;
    ASSERT_TRUE(parser.Parse(wrap));
    ASSERT_TRUE(IsTextureWrap(wrap, kDefaultWrapMode, kDefaultWrapMode, kRepeat));
  }

  TEST_F(JsonTextureWrapParserTest, Test_Parse_WillPass_StringEqualsDefault) {
    json::Document doc;
    json::ParseJson(doc, "\"default\"");
    JsonTextureWrapParser parser((const json::Value&) doc);

    TextureWrap wrap;
    ASSERT_TRUE(parser.Parse(wrap));
    ASSERT_TRUE(IsTextureWrap(wrap, kDefaultWrapMode));
  }

  TEST_F(JsonTextureWrapParserTest, Test_Parse_WillPass_StringEqualsRepeat) {
    json::Document doc;
    json::ParseJson(doc, "\"repeat\"");
    JsonTextureWrapParser parser((const json::Value&) doc);

    TextureWrap wrap;
    ASSERT_TRUE(parser.Parse(wrap));
    ASSERT_TRUE(IsTextureWrap(wrap, kRepeat));
  }

  TEST_F(JsonTextureWrapParserTest, Test_Parse_WillPass_StringEqualsClampToEdge) {
    json::Document doc;
    json::ParseJson(doc, "\"clamp2edge\"");
    JsonTextureWrapParser parser((const json::Value&) doc);

    TextureWrap wrap;
    ASSERT_TRUE(parser.Parse(wrap));
    ASSERT_TRUE(IsTextureWrap(wrap, kClampToEdge));
  }

  TEST_F(JsonTextureWrapParserTest, Test_Parse_WillPass_StringEqualsClampToBorder) {
    json::Document doc;
    json::ParseJson(doc, "\"clamp2border\"");
    JsonTextureWrapParser parser((const json::Value&) doc);

    TextureWrap wrap;
    ASSERT_TRUE(parser.Parse(wrap));
    ASSERT_TRUE(IsTextureWrap(wrap, kClampToBorder));
  }
}