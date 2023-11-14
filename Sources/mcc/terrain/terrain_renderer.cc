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
    const auto material = GetMaterial(Terrain::GetTerrainMaterial());

    GetSelectedTexture().Bind0();
    shader_.ApplyShader();
    shader_.SetUniformBlock("Camera", 0);
    shader_.SetUniformBlock("DirectionalLight", 1);
    shader_.SetUniformBlock("PointLight", 2);
    shader_.SetMat4("model", chunk->GetModelMatrix());
    shader_.SetInt("tex", 0);
    shader_.SetMaterial("material", material);
    shader_.ApplyShader();
    chunk->Render();
  }
}