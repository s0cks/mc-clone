#include <gtest/gtest.h>

#include "mcc/program/program_loader_json.h"

namespace mcc::program {
  using namespace ::testing;

  class JsonProgramLoaderTest : public Test {
  protected:
    JsonProgramLoaderTest() = default;
  public:
    ~JsonProgramLoaderTest() override = default;
  };

  TEST_F(JsonProgramLoaderTest, Test_InvalidProgram1) {
    static const uri::Uri kProgramUri = uri::Uri("program://invalid-program1");
    JsonProgramLoader loader(kProgramUri);
    const auto program = loader.LoadProgram();
    ASSERT_EQ(program, nullptr);
  }

  TEST_F(JsonProgramLoaderTest, Test_InvalidProgram2) {
    static const uri::Uri kProgramUri = uri::Uri("program://invalid-program2");
    JsonProgramLoader loader(kProgramUri);
    const auto program = loader.LoadProgram();
    ASSERT_EQ(program, nullptr);
  }

  TEST_F(JsonProgramLoaderTest, Test_InvalidProgram3) {
    static const uri::Uri kProgramUri = uri::Uri("program://invalid-program3");
    JsonProgramLoader loader(kProgramUri);
    const auto program = loader.LoadProgram();
    ASSERT_EQ(program, nullptr);
  }

  TEST_F(JsonProgramLoaderTest, Test_TestProgram1) {
    static const uri::Uri kProgramUri = uri::Uri("program://test-program1");
    JsonProgramLoader loader(kProgramUri);
    const auto program = loader.LoadProgram();
    ASSERT_NE(program, nullptr);
  }
}