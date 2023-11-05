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

    auto camera = camera::PerspectiveCameraBehavior::GetCameraComponent();

    const float globalLightIntensity = 0.0f;
    const glm::vec3 globalLightColor(1.0f, 1.0f, 1.0f);

    glm::vec3 lightPos(1.0f);
    glm::vec3 lightColor(1.0f);
    float lightIntensity = 0.0f;
    AmbientLight::Visit([&](const Entity& e, const ComponentState<AmbientLight>& light) {
      const auto transform = physics::Transform::GetState(e);
      if(!transform) {
        LOG(ERROR) << "no physics::Transform component found for entity " << e;
        return false;
      }

      lightPos = (*transform)->position;
      lightColor = light->color;
      lightIntensity = light->intensity;
      return true;
    });

    GetSelectedTexture().Bind0();
    shader_.ApplyShader();
    shader_.SetMat4("projection", projection_);
    shader_.SetMat4("view", view_);
    shader_.SetMat4("model", chunk->GetModelMatrix());
    shader_.SetInt("tex0", 0);
    shader_.SetVec3("cameraPos", (*camera)->pos);
    shader_.SetVec3("lightPos", lightPos);
    shader_.SetVec3("lightColor", lightColor);
    shader_.SetFloat("lightIntensity", lightIntensity);
    
    shader_.SetVec3("globalLightColor", globalLightColor);
    shader_.SetFloat("globalLightIntensity", globalLightIntensity);

    shader_.ApplyShader();
    chunk->Render();
  }
}