#include "mcc/terrain/terrain.h"
#include "mcc/engine/engine.h"
#include "mcc/shader/shader.h"
#include "mcc/texture/texture.h"
#include "mcc/thread_local.h"
#include "mcc/flags.h"

#include "mcc/terrain/terrain_renderer.h"
#include "mcc/ecs/component.h"
#include "mcc/lighting/ambient_light.h"
#include "mcc/physics/transform.h"

#include "mcc/camera/perspective_camera.h"

namespace mcc::terrain {
  DEFINE_uint64(terrain_size, 128, "The size of the terrain.");

  static ThreadLocal<TerrainChunk> chunk_;
  
  void Terrain::SetChunk(TerrainChunk* chunk) {
    chunk_.Set(chunk);
  }

  TerrainChunk* Terrain::GetChunk() {
    return chunk_.Get();
  }

  void Terrain::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);

    TerrainRenderer::Init();
    TerrainChunk::Init();
  }

  void Terrain::OnPreInit() {

  }

  void Terrain::OnInit() {

  }

  void Terrain::OnPostInit() {
    
  }

  void Terrain::Render(const glm::mat4 projection, const glm::mat4 view) {
    TerrainRenderer renderer(projection, view);
    renderer.Render();
  }

  static RelaxedAtomic<TerrainTexture> tex_(TerrainTexture::kDefaultTerrainTexture);

  void Terrain::SetTexture(const TerrainTexture texture) {
    tex_ = texture;
  }

  TerrainTexture Terrain::GetTexture() {
    return (const TerrainTexture&) tex_;
  }
}