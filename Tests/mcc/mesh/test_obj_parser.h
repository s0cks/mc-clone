#ifndef MCC_TEST_OBJ_PARSER_H
#define MCC_TEST_OBJ_PARSER_H

#include <gtest/gtest.h>
#include "mcc/mesh/obj_parser.h"

namespace mcc {
  class ObjParserTest : public ::testing::Test {
  protected:
    ObjParserTest() = default;
  public:
    ~ObjParserTest() override = default;
  };
}

#endif //MCC_TEST_OBJ_PARSER_H