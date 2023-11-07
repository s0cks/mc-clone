#ifndef MCC_MESH_TEST_MESH_H
#define MCC_MESH_TEST_MESH_H

#include <gtest/gtest.h>
#include "mcc/mock_gl.h"
#include "mcc/mesh/mesh.h"

namespace mcc::mesh {
  class MeshTest : public ::testing::Test {
  protected:
    MeshTest() = default;
  public:
    ~MeshTest() override = default;

    void SetUp() override {
      MockGl gl;
      EXPECT_CALL(gl, glGenVertexArrays(::testing::Eq(5), ::testing::_));
      Mesh::InitializeBuiltinVaos();
    }
  };
}

#endif //MCC_MESH_TEST_MESH_H