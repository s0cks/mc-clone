#include "mcc/shape/cube.h"

namespace mcc {
  static const mesh::VertexList kCubeVertices = {
    { .pos = glm::vec3(-0.5f, -0.5f, -0.5f), .uv = glm::vec2(0.0f, 0.0f), },
    { .pos = glm::vec3( 0.5f, -0.5f, -0.5f), .uv = glm::vec2(1.0f, 0.0f), },
    { .pos = glm::vec3(0.5f,  0.5f, -0.5f), .uv = glm::vec2(1.0f, 1.0f), },
    { .pos = glm::vec3(0.5f,  0.5f, -0.5f), .uv = glm::vec2(1.0f, 1.0f), },
    { .pos = glm::vec3(-0.5f,  0.5f, -0.5f), .uv = glm::vec2(0.0f, 1.0f), },
    { .pos = glm::vec3(-0.5f, -0.5f, -0.5f), .uv = glm::vec2(0.0f, 0.0f), },

    { .pos = glm::vec3(-0.5f, -0.5f,  0.5f), .uv = glm::vec2(0.0f, 0.0f), },
    { .pos = glm::vec3(0.5f, -0.5f,  0.5f), .uv = glm::vec2(1.0f, 0.0f), },
    { .pos = glm::vec3(0.5f,  0.5f,  0.5f), .uv = glm::vec2(1.0f, 1.0f), },
    { .pos = glm::vec3(0.5f,  0.5f,  0.5f), .uv = glm::vec2(1.0f, 1.0f), },
    { .pos = glm::vec3(-0.5f,  0.5f,  0.5f), .uv = glm::vec2(0.0f, 1.0f), },
    { .pos = glm::vec3(-0.5f, -0.5f,  0.5f), .uv = glm::vec2(0.0f, 0.0f), },

    { .pos = glm::vec3(-0.5f,  0.5f,  0.5f), .uv = glm::vec2(1.0f, 0.0f), },
    { .pos = glm::vec3(-0.5f,  0.5f, -0.5f), .uv = glm::vec2(1.0f, 1.0f), },
    { .pos = glm::vec3(-0.5f, -0.5f, -0.5f), .uv = glm::vec2(0.0f, 1.0f), },
    { .pos = glm::vec3(-0.5f, -0.5f, -0.5f), .uv = glm::vec2(0.0f, 1.0f), },
    { .pos = glm::vec3(-0.5f, -0.5f,  0.5f), .uv = glm::vec2(0.0f, 0.0f), },
    { .pos = glm::vec3(-0.5f,  0.5f,  0.5f), .uv = glm::vec2(1.0f, 0.0f), },

    { .pos = glm::vec3(0.5f,  0.5f,  0.5f), .uv = glm::vec2(1.0f, 0.0f), },
    { .pos = glm::vec3(0.5f,  0.5f, -0.5f), .uv = glm::vec2(1.0f, 1.0f), },
    { .pos = glm::vec3(0.5f, -0.5f, -0.5f), .uv = glm::vec2(0.0f, 1.0f), },
    { .pos = glm::vec3(0.5f, -0.5f, -0.5f), .uv = glm::vec2(0.0f, 1.0f), },
    { .pos = glm::vec3(0.5f, -0.5f,  0.5f), .uv = glm::vec2(0.0f, 0.0f), },
    { .pos = glm::vec3(0.5f,  0.5f,  0.5f), .uv = glm::vec2(1.0f, 0.0f), },

    { .pos = glm::vec3(-0.5f, -0.5f, -0.5f), .uv = glm::vec2(0.0f, 1.0f), },
    { .pos = glm::vec3(0.5f, -0.5f, -0.5f), .uv = glm::vec2(1.0f, 1.0f), },
    { .pos = glm::vec3(0.5f, -0.5f,  0.5f), .uv = glm::vec2(1.0f, 0.0f), },
    { .pos = glm::vec3(0.5f, -0.5f,  0.5f), .uv = glm::vec2(1.0f, 0.0f), },
    { .pos = glm::vec3(-0.5f, -0.5f,  0.5f), .uv = glm::vec2(0.0f, 0.0f), },
    { .pos = glm::vec3(-0.5f, -0.5f, -0.5f), .uv = glm::vec2(0.0f, 1.0f), },

    { .pos = glm::vec3(-0.5f,  0.5f, -0.5f), .uv = glm::vec2(0.0f, 1.0f), },
    { .pos = glm::vec3(0.5f,  0.5f, -0.5f), .uv = glm::vec2(1.0f, 1.0f), },
    { .pos = glm::vec3(0.5f,  0.5f,  0.5f), .uv = glm::vec2(1.0f, 0.0f), },
    { .pos = glm::vec3(0.5f,  0.5f,  0.5f), .uv = glm::vec2(1.0f, 0.0f), },
    { .pos = glm::vec3(-0.5f,  0.5f,  0.5f), .uv = glm::vec2(0.0f, 0.0f), },
    { .pos = glm::vec3(-0.5f,  0.5f, -0.5f), .uv = glm::vec2(0.0f, 1.0f), },
  };
  
  mesh::Mesh* Cube::CreateMesh() {
    return mesh::NewMesh(kCubeVertices);
  }
}