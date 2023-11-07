#include "mcc/mesh/test_mesh.h"
#include "mcc/mock_gl.h"

namespace mcc::mesh {
  using namespace ::testing;

  TEST_F(MeshTest, TestRender) {
    MockGl gl;
    EXPECT_CALL(gl, glGenVertexArrays(Eq(5), _));
    const auto mesh = mesh::NewCube();

    EXPECT_CALL(gl, glDrawArrays(_, _, _));
    mesh->Render();
  }
}