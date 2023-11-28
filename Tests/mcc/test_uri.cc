#include <regex>
#include <gtest/gtest.h>

#include "mcc/uri.h"

namespace mcc::uri {
  class ParserTest : public ::testing::Test {
  protected:
    ParserTest() = default;
  public:
    ~ParserTest() override = default;
  };

  TEST_F(ParserTest, Test_ParseFails_InvalidScheme1) {
    Parser parser("tex:/test.png");
    ASSERT_FALSE(parser.Parse());
  }

  TEST_F(ParserTest, Test_ParseFails_InvalidScheme) {
    Parser parser(":test.png");
    ASSERT_FALSE(parser.Parse());
  }

  TEST_F(ParserTest, Test_Parse0) {
    Parser parser("t:test");
    ASSERT_TRUE(parser.Parse());
  }

  TEST_F(ParserTest, Test_Parse1) {
    Parser parser("t://test");
    ASSERT_TRUE(parser.Parse());
  }

  TEST_F(ParserTest, Test_Parse2) {
    Parser parser("tex://test.png");
    ASSERT_TRUE(parser.Parse());
  }

  TEST_F(ParserTest, Test_Parse3) {
    Parser parser("tex://test.png?");
    ASSERT_TRUE(parser.Parse());
  }

  TEST_F(ParserTest, Test_Parse4) {
    Parser parser("tex://test.png?test");
    ASSERT_TRUE(parser.Parse());
  }

  TEST_F(ParserTest, Test_Parse5) {
    Parser parser("tex://test.png?message=hello");
    ASSERT_TRUE(parser.Parse());
  }

  TEST_F(ParserTest, Test_Parse6) {
    Parser parser("tex://test.png?message=hello&");
    ASSERT_TRUE(parser.Parse());
  }

  TEST_F(ParserTest, Test_Parse7) {
    Parser parser("tex://test.png?message=hello&test");
    ASSERT_TRUE(parser.Parse());
  }

  TEST_F(ParserTest, Test_Parse8) {
    Parser parser("tex://test.png?message=hello&test#test");
    ASSERT_TRUE(parser.Parse());
  }

  class UriTest : public ::testing::Test {
  protected:
    UriTest() = default;
  public:
    ~UriTest() override = default;
  };

  TEST_F(UriTest, Test_Parse_ExplicitProtocol) {
    Uri uri("test://hello-world");
    ASSERT_EQ(uri.scheme, "test");
    ASSERT_EQ(uri.path, "hello-world");
  }
}