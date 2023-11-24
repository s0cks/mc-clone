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
    static const std::string kProtocol = "test";
    static const std::string kLocation = "location1";

    static const std::string kTestUri = std::string() + kProtocol + "://" + kLocation;

    Uri uri(kProtocol + "://" + kLocation);
    ASSERT_EQ(uri.protocol, kProtocol);
    ASSERT_EQ(uri.location, kLocation);
  }

  TEST_F(UriTest, Test_Parse_ImplicitProtocol) {
    static const std::string kTestUri = "location1";

    Uri uri(kTestUri);
    ASSERT_EQ(uri.protocol, "file");
    ASSERT_EQ(uri.location, kTestUri);
  }
}