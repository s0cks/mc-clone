#include "mcc/terrain/terrain.h"
#include "mcc/engine/engine.h"
#include "mcc/shader/shader.h"
#include "mcc/texture/texture.h"
#include "mcc/thread_local.h"
#include "mcc/flags.h"

#include "mcc/lighting/ambient_light.h"
#include "mcc/physics/transform.h"
#include "mcc/camera/perspective_camera.h"

namespace mcc::terrain {
  DEFINE_uint64(terrain_size, 128, "The size of the terrain.");

  static rxsub::subject<TerrainChunk*> chunk_subject_;
  static ThreadLocal<TerrainChunk> chunk_;
  static ShaderRef shader_;
  static MaterialRef material_;
  static rxsub::subject<MaterialRef> material_subject_;

  MaterialRef Terrain::GetTerrainMaterial() {
    return material_;
  }

  std::string Terrain::GetTerrainMaterialName() {
    return material_->name();
  }

  void Terrain::SetTerrainMaterial(MaterialRef material) {
    material_ = material;
    material_subject_.get_subscriber().on_next(material);
  }
  
  void Terrain::SetChunk(TerrainChunk* chunk) {
    chunk_.Set(chunk);
    chunk_subject_.get_subscriber().on_next(chunk);
  }

  TerrainChunk* Terrain::GetChunk() {
    return chunk_.Get();
  }

  void Terrain::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
    TerrainChunk::Init();
  }

  void Terrain::OnPreInit() {

  }

  void Terrain::OnInit() {
  }

  void Terrain::OnPostInit() {
    SetTerrainMaterial(GetMaterial("material:floors/old_wood"));
  }

  rx::observable<TerrainChunk*> Terrain::GetChunkObservable() {
    return chunk_subject_.get_observable();
  }

  rx::observable<MaterialRef> Terrain::GetMaterialObservable() {
    return material_subject_.get_observable();
  }
}