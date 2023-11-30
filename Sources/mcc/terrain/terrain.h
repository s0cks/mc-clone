#ifndef MCC_TERRAIN_H
#define MCC_TERRAIN_H

#include <vector>
#include <iostream>

#include "mcc/rx.h"
#include "mcc/gfx.h"
#include "mcc/vao.h"
#include "mcc/common.h"
#include "mcc/material/material.h"
#include "mcc/terrain/terrain_chunk.h"

namespace mcc::renderer {
  class Renderer;
  class RenderTerrainStage;
}

namespace mcc::terrain {
  class Terrain {
    friend class TerrainChunk;
    friend class mcc::renderer::RenderTerrainStage;
    DEFINE_NON_COPYABLE_TYPE(Terrain);
  private:

  public:
    Terrain() = delete;
    virtual ~Terrain() = default;

  private:
    static void OnPreInit();
    static void OnInit();
    static void OnPostInit();

    static void SetChunk(TerrainChunk* chunk);
  public:
    static void Init();
    static VertexArrayObject GetVao();
    static TerrainVertexBuffer GetVbo();
    static IndexBuffer GetIbo();
    static TerrainChunk* GetChunk();
    static MaterialRef GetTerrainMaterial();
    static std::string GetTerrainMaterialName();
    static void SetTerrainMaterial(MaterialRef material);
    
    static inline void
    SetTerrainMaterial(const std::string& name) {
      return SetTerrainMaterial(GetMaterial(name));
    }
    
    static rx::observable<TerrainChunk*> GetChunkObservable();
    static rx::observable<MaterialRef> GetMaterialObservable();
  };
}

#endif //MCC_TERRAIN_H