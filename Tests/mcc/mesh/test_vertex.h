#ifndef MCC_TEST_VERTEX_H
#define MCC_TEST_VERTEX_H

#include <gtest/gtest.h>
#include "mcc/mesh/vertex.h"

namespace mcc {
  class VertexTest : public ::testing::Test {
  protected:
    VertexTest() = default;
  public:
    ~VertexTest() override = default;
  };
}

#endif //MCC_TEST_VERTEX_H