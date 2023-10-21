#include "mcc/shape/cube.h"

namespace mcc {
  static const VertexList kVertices = {
    { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), },
    { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f), },
    { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), },
    { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), },
    { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f), },
    { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), },

    { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), },
    { glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec2(1.0f, 0.0f), },
    { glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f), },
    { glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f), },
    { glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(0.0f, 1.0f), },
    { glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f), },

    { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), },
    { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), },
    { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), },
    { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), },
    { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), },
    { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), },

    { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), },
    { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), },
    { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), },
    { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), },
    { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), },
    { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), },

    { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), },
    { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f), },
    { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f), },
    { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f), },
    { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), },
    { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), },

    { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f), },
    { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), },
    { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), },
    { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), },
    { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f), },
    { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f), },
  };
  
  mesh::Mesh* Cube::CreateMesh() {
    return mesh::Mesh::New(kVertices);
  }
}