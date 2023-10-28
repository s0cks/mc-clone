#include "mcc/mesh/test_vertex.h"

namespace mcc {
  TEST_F(VertexTest, TestSize) {
    ASSERT_EQ(sizeof(Vertex), 14 * sizeof(float));
  }
}