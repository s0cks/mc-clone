#include "mcc/skybox.h"
#include "mcc/mesh/mesh.h"
#include "mcc/engine/engine.h"
#include "mcc/shader/shader.h"
#include "mcc/thread_local.h"

namespace mcc::skybox {
  static VertexArrayObject vao_(kInvalidVertexArrayObject);
  static ThreadLocal<Skybox> skybox_;

  void Skybox::OnPostInit() {
    vao_ = VertexArrayObject();
    skybox_.Set(Skybox::New(GetTexture("space_nebulas"), GetShader("skybox")));
  }

  void Skybox::Init() {
    Engine::OnPostInit(&OnPostInit);
  }

  Skybox* Skybox::Get() {
    return skybox_.Get();
  }

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

  Skybox::Skybox(TextureRef t, ShaderRef s):
    vao(vao_),
    vbo(kSkyboxVertices),
    texture(t),
    shader(s) {
  }

  Skybox* Skybox::New(TextureRef texture, ShaderRef shader) {
    VertexArrayObjectScope scope(vao_);
    return new Skybox(texture, shader);
  }
}