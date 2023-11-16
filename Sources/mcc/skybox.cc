#include "mcc/skybox.h"
#include "mcc/mesh/mesh.h"
#include "mcc/engine/engine.h"
#include "mcc/shader/shader.h"
#include "mcc/thread_local.h"

namespace mcc::skybox {
  static VertexArrayObject vao_(kInvalidVertexArrayObject);
  static ShaderRef shader_;
  static ThreadLocal<Skybox> skybox_;

  void Skybox::OnPostInit() {
    vao_ = VertexArrayObject();
    shader_ = GetShader("skybox");
    skybox_.Set(Skybox::New(FLAGS_resources + "/textures/skybox/graycloud"));
  }

  void Skybox::Init() {
    Engine::OnPostInit(&OnPostInit);
  }

  Skybox* Skybox::Get() {
    return skybox_.Get();
  }

  static const VertexList kSkyboxVertices = {
    // positions          
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

  Skybox* Skybox::New(const std::string& cube_map_filename) {
    VertexArrayObjectScope scope(vao_);
    return new Skybox(cube_map_filename, kSkyboxVertices);
  }

  void Skybox::Render() {
    glDisable(GL_CULL_FACE);
    CHECK_GL(FATAL);
    glEnable(GL_DEPTH_TEST);
    CHECK_GL(FATAL);
    glDepthFunc(GL_LEQUAL);
    CHECK_GL(FATAL);
    glActiveTexture(GL_TEXTURE0);
    CHECK_GL(FATAL);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_.id());
    CHECK_GL(FATAL);
    shader_->ApplyShader();
    shader_->SetUniformBlock("Camera", 0);
    shader_->SetInt("tex", 0);
    shader_->ApplyShader();
    VertexArrayObjectScope scope(vao_);
    glDrawArrays(GL_TRIANGLES, 0, vbo_.length());
    CHECK_GL(FATAL);
    glDepthFunc(GL_LESS);
    CHECK_GL(FATAL);
    glDisable(GL_DEPTH_TEST);
    CHECK_GL(FATAL);
    glEnable(GL_CULL_FACE);
    CHECK_GL(FATAL);
  }
}