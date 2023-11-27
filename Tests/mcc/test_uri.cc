#include <regex>
#include <gtest/gtest.h>

#include "mcc/uri.h"

namespace mcc::uri {
  class TestProtocol : public Protocol {
  private:
    std::regex pattern_;
  public:
    TestProtocol():
      Protocol(),
      pattern_("^test:(?:\\/\\/)?.*$") {
    }
    ~TestProtocol() override = default;

    std::string GetName() const override {
      return "test";
    }

    bool Matches(const uri::basic_uri& uri) const override {
      return std::regex_match(uri, pattern_);
    }

    std::string Resolve(const uri::Uri& uri) const override {
      return uri.location;
    }
  };

  class UriTest : public ::testing::Test {
  protected:
    UriTest() = default;
  public:
    ~UriTest() override = default;

    void SetUp() override {
      uri::RegisterProtocol<TestProtocol>();
    }
  };

  TEST_F(UriTest, Test_Parse_ExplicitProtocol) {
    Uri uri("test://hello-world");
    ASSERT_EQ(uri.protocol, "test");
    ASSERT_EQ(uri.location, "hello-world");
  }

  class UriParserTest : public ::testing::Test {
  protected:
    UriParserTest() = default;
  public:
    ~UriParserTest() override = default;
  };

  TEST_F(UriParserTest, Test_ParseFails_InvalidScheme1) {
    UriParser parser("tex:/test.png");
    ASSERT_FALSE(parser.Parse());
  }

  TEST_F(UriParserTest, Test_ParseFails_InvalidScheme) {
    UriParser parser(":test.png");
    ASSERT_FALSE(parser.Parse());
  }

  TEST_F(UriParserTest, Test_Parse0) {
    UriParser parser("t:test");
    ASSERT_TRUE(parser.Parse());
  }

  TEST_F(UriParserTest, Test_Parse1) {
    UriParser parser("t://test");
    ASSERT_TRUE(parser.Parse());
  }

  TEST_F(UriParserTest, Test_Parse2) {
    UriParser parser("tex://test.png");
    ASSERT_TRUE(parser.Parse());
  }

  TEST_F(UriParserTest, Test_Parse3) {
    UriParser parser("tex://test.png?");
    ASSERT_TRUE(parser.Parse());
  }

  TEST_F(UriParserTest, Test_Parse4) {
    UriParser parser("tex://test.png?test");
    ASSERT_TRUE(parser.Parse());
  }

  TEST_F(UriParserTest, Test_Parse5) {
    UriParser parser("tex://test.png?message=hello");
    ASSERT_TRUE(parser.Parse());
  }

  TEST_F(UriParserTest, Test_Parse6) {
    UriParser parser("tex://test.png?message=hello&");
    ASSERT_TRUE(parser.Parse());
  }

  TEST_F(UriParserTest, Test_Parse7) {
    UriParser parser("tex://test.png?message=hello&test");
    ASSERT_TRUE(parser.Parse());
  }

  TEST_F(UriParserTest, Test_Parse8) {
    UriParser parser("tex://test.png?message=hello&test#test");
    ASSERT_TRUE(parser.Parse());
  }
}