#include "mcc/skybox/skybox_vertex.h"

namespace mcc::skybox {
  static const VertexList kSkyboxVertices = {       
    { .pos = glm::vec3(-1.0f,  1.0f, -1.0f), },
    { .pos = glm::vec3(-1.0f, -1.0f, -1.0f), },
    { .pos = glm::vec3(1.0f, -1.0f, -1.0f), },
    { .pos = glm::vec3(1.0f, -1.0f, -1.0f), },
    { .pos = glm::vec3(1.0f,  1.0f, -1.0f), },
    { .pos = glm::vec3(-1.0f,  1.0f, -1.0f), },

    { .pos = glm::vec3(-1.0f, -1.0f,  1.0f), },
    { .pos = glm::vec3(-1.0f, -1.0f, -1.0f), },
    { .pos = glm::vec3(-1.0f,  1.0f, -1.0f), },
    { .pos = glm::vec3(-1.0f,  1.0f, -1.0f), },
    { .pos = glm::vec3(-1.0f,  1.0f,  1.0f), },
    { .pos = glm::vec3(-1.0f, -1.0f,  1.0f), },

    { .pos = glm::vec3(1.0f, -1.0f, -1.0f), },
    { .pos = glm::vec3(1.0f, -1.0f,  1.0f), },
    { .pos = glm::vec3(1.0f,  1.0f,  1.0f), },
    { .pos = glm::vec3(1.0f,  1.0f,  1.0f), },
    { .pos = glm::vec3(1.0f,  1.0f, -1.0f), },
    { .pos = glm::vec3(1.0f, -1.0f, -1.0f), },

    { .pos = glm::vec3(-1.0f, -1.0f,  1.0f), },
    { .pos = glm::vec3(-1.0f,  1.0f,  1.0f), },
    { .pos = glm::vec3(1.0f,  1.0f,  1.0f), },
    { .pos = glm::vec3(1.0f,  1.0f,  1.0f), },
    { .pos = glm::vec3(1.0f, -1.0f,  1.0f), },
    { .pos = glm::vec3(-1.0f, -1.0f,  1.0f), },

    { .pos = glm::vec3(-1.0f,  1.0f, -1.0f), },
    { .pos = glm::vec3(1.0f,  1.0f, -1.0f), },
    { .pos = glm::vec3(1.0f,  1.0f,  1.0f), },
    { .pos = glm::vec3(1.0f,  1.0f,  1.0f), },
    { .pos = glm::vec3(-1.0f,  1.0f,  1.0f), },
    { .pos = glm::vec3(-1.0f,  1.0f, -1.0f), },

    { .pos = glm::vec3(-1.0f, -1.0f, -1.0f), },
    { .pos = glm::vec3(-1.0f, -1.0f,  1.0f), },
    { .pos = glm::vec3(1.0f, -1.0f, -1.0f), },
    { .pos = glm::vec3(1.0f, -1.0f, -1.0f), },
    { .pos = glm::vec3(-1.0f, -1.0f,  1.0f), },
    { .pos = glm::vec3(1.0f, -1.0f,  1.0f), },
  };

  void GenerateSkyboxVertices(VertexList& vertices) {
    vertices.clear();
    vertices.insert(std::end(vertices), std::begin(kSkyboxVertices), std::end(kSkyboxVertices));
  }
}