#include "mcc/mesh/test_obj_parser.h"
#include "mcc/mesh/mesh_loader.h"
#include "mcc/flags.h"
#include "mcc/mock_gl.h"

namespace mcc {
  TEST_F(ObjParserTest, Test_Parse_Sphere) {
    const auto filename = FLAGS_resources + "/meshes/sphere/sphere.obj";
    const auto file = fopen(filename.c_str(), "rb");
    ASSERT_NE(file, nullptr);
    ObjParser parser(file);
    ASSERT_TRUE(parser.ParseFile());
    fclose(file);
  }

  TEST_F(ObjParserTest, Test_Parse_Cube) {
    const auto filename = FLAGS_resources + "/meshes/cube/cube.obj";
    const auto file = fopen(filename.c_str(), "rb");
    ASSERT_NE(file, nullptr);
    ObjParser parser(file);
    ASSERT_TRUE(parser.ParseFile());
    fclose(file);
  }
}