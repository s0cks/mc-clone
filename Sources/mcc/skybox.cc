#include "mcc/skybox.h"
#include "mcc/mesh/mesh.h"
#include "mcc/engine/engine.h"
#include "mcc/shader/shader.h"
#include "mcc/thread_local.h"

#include "mcc/renderer/renderer.h"

namespace mcc::skybox {
  static Vao kSkyboxVao;
  static ThreadLocal<Skybox> skybox_;
  static rxsub::subject<Skybox*> skybox_subject_;

  void Skybox::OnPostInit() {
    kSkyboxVao = VertexArrayObject::New();
    SetSkybox(Skybox::New(CubeMap::New("space_nebulas"), Program::New("skybox")));
  }

  void Skybox::Init() {
    //TODO: Engine::OnPostInit(&OnPostInit);
  }

  void Skybox::SetSkybox(Skybox* skybox) {
    skybox_.Set(skybox);
    skybox_subject_.get_subscriber().on_next(skybox);
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

  Skybox::Skybox(TextureRef t, ProgramRef s):
    vao(kSkyboxVao),
    vbo(kSkyboxVertices),
    texture(t),
    shader(s) {
  }

  Skybox* Skybox::New(TextureRef texture, ProgramRef shader) {
    VertexArrayObjectScope scope(kSkyboxVao);
    return new Skybox(texture, shader);
  }

  rx::observable<Skybox*> Skybox::GetObservable() {
    return skybox_subject_.get_observable();
  }

  void RenderSkyboxPipeline::Render() {
    if(!skybox_)
      return;

    const auto& texture = skybox_->texture;
    const auto& vao = skybox_->vao;
    InvertedCullFaceScope cull_face;
    DepthTestScope depth_test(gfx::kLequal);
    TextureBindScope<0> tex(texture);
    RenderChildren();
    //TODO: renderer::Renderer::IncrementVertexCounter(skybox_->vbo.length());
  }
}