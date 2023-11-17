#ifndef MCC_TEST_RAPIDJSON_H
#define MCC_TEST_RAPIDJSON_H

#include <gtest/gtest.h>
#include "mcc/json.h"

namespace mcc {
  class RapidJsonTest : public ::testing::Test {
  protected:
    RapidJsonTest() = default;
  public:
    ~RapidJsonTest() override = default;
  };
}

#endif //MCC_TEST_RAPIDJSON_H