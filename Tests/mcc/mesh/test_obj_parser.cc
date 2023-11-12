#include "mcc/mesh/test_obj_parser.h"
#include "mcc/mesh/mesh_loader.h"
#include "mcc/flags.h"
#include "mcc/mock_gl.h"

namespace mcc {
  TEST_F(ObjParserTest, Test_Parse) {
    const auto filename = FLAGS_resources + "/meshes/sphere/sphere.obj";
    const auto vao = VertexArrayObject(kInvalidVertexArrayObject);

    MockGl gl;

    mesh::ObjMeshLoader loader(vao, filename);
    auto mesh = loader.LoadMesh();
    ASSERT_NE(mesh, nullptr);
  }
}