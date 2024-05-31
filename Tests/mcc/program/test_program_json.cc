#include <gtest/gtest.h>

#include "mcc/program/program_json.h"

namespace mcc::program {
  using namespace ::testing;

  class ProgramJsonTest : public Test {
  protected:
    ProgramJsonTest() = default;
  public:
    ~ProgramJsonTest() override = default;
  };

  template<class Handler>
  static inline AssertionResult
  ParseIsSuccessful(const char* json, Handler& handler) {
    rapidjson::Reader reader;
    rapidjson::StringStream ss(json);
    if(reader.Parse(ss, handler))
      return AssertionSuccess();
    const auto code = reader.GetParseErrorCode();
    const auto offset = reader.GetErrorOffset();
    return AssertionFailure() << "expected \"" << json << "\" to be valid json, received: \"" << rapidjson::GetParseError_En(code) << "\" at " << offset << " near: " << std::string(json, 10) << "....";
  }

  TEST_F(ProgramJsonTest, Test_SAX) {
    static constexpr const auto kTestJson = 
      "{"
      "\"vertex\": \"test.vert\","
      "\"fragment\": \"test.frag\""
      "}";
    
    ProgramReaderHandler handler;
    ASSERT_TRUE(ParseIsSuccessful(kTestJson, handler));

    DLOG(INFO) << "shaders:";
    for(const auto& shader : handler.shaders()) {
      DLOG(INFO) << " - " << shader;
    }
  }
}