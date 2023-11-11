#include "mcc/terrain/terrain_renderer.h"
#include "mcc/terrain/terrain.h"
#include "mcc/texture/texture.h"
#include "mcc/relaxed_atomic.h"
#include "mcc/flags.h"

#include "mcc/lighting/ambient_light.h"
#include "mcc/physics/transform.h"

#include "mcc/camera/perspective_camera.h"
#include "mcc/shader/shader.h"

namespace mcc::terrain {
  static shader::Shader shader_;
  static texture::Texture textures_[kNumberOfTerrainTextures];

  static inline texture::Texture
  GetSelectedTexture() {
    return textures_[Terrain::GetTexture()];
  }

  void TerrainRenderer::Init() {
    Engine::OnPostInit(&OnPostInit);
  }

  void TerrainRenderer::OnPostInit() {
#define LOAD_TERRAIN_TEXTURE(Name, File) \
    textures_[k##Name##Texture] = texture::Texture::LoadFrom(FLAGS_resources + "/textures/" + (File));

    FOR_EACH_TERRAIN_TEXTURE(LOAD_TERRAIN_TEXTURE)
#undef LOAD_TERRAIN_TEXTURE
    shader_ = shader::Shader::Get("terrain");
  }

  void TerrainRenderer::Render() {
    glm::mat4 model(1.0f);
    const auto chunk = Terrain::GetChunk();

    glm::vec3 lightColor = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 lightPos = glm::vec3(0.0f);
    AmbientLight::Visit([&lightColor,&lightPos](const Entity& l, const ComponentState<AmbientLight>& light) {
      lightColor = light->color;

      const auto lt = physics::Transform::GetState(l);
      LOG_IF(FATAL, !lt) << "no transform found for AmbientLight component of entity " << l;
      lightPos = (*lt)->position;
      return true;
    });
    const auto& cameraPos = (*camera::PerspectiveCameraBehavior::GetCameraComponent())->pos;

    const auto material = GetMaterial(Terrain::GetTerrainMaterial());

    GetSelectedTexture().Bind0();
    shader_.ApplyShader();
    shader_.SetMat4("projection", projection_);
    shader_.SetMat4("view", view_);
    shader_.SetMat4("model", chunk->GetModelMatrix());
    shader_.SetInt("tex", 0);
    shader_.SetCamera("camera", cameraPos);
    const auto diffuseColor = lightColor * glm::vec3(0.5f);
    const auto ambientColor = diffuseColor * glm::vec3(0.2f);
    shader_.SetLight("light", lightPos, ambientColor, diffuseColor, glm::vec3(1.0f));
    shader_.SetVec3("lightColor", lightColor);
    shader_.SetMaterial("material", material);

    shader_.ApplyShader();
    chunk->Render();
  }
}