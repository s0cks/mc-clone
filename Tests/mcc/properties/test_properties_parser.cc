#include <gtest/gtest.h>
#include <fmt/format.h>

#include "mcc/flags.h"
#include "mcc/properties/properties_parser.h"

namespace mcc::properties {
  using namespace ::testing;

  class PropertiesParserTest : public Test {
  protected:
    PropertiesParserTest() = default;
  public:
    ~PropertiesParserTest() override = default;
  };

  TEST_F(PropertiesParserTest, Test_Parse) {
    static constexpr const auto kTestPropertiesFileName = "test1.properties";

    const auto filename = fmt::format("{0:s}/{1:s}", FLAGS_resources, kTestPropertiesFileName);
    MCC_ASSERT(FileExists(filename));
    const auto file = fopen(filename.c_str(), "rb");
    Parser parser(file);
    
    PropertySet properties;
    ASSERT_TRUE(parser.ParseProperties(properties));
  }
}