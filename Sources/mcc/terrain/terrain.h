#ifndef MCC_TERRAIN_H
#define MCC_TERRAIN_H

#include <vector>
#include <iostream>

#include "mcc/gfx.h"
#include "mcc/common.h"
#include "mcc/material.h"
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
    static void Render();
  public:
    static void Init();
    static VertexArrayObject GetVao();
    static TerrainVertexBuffer GetVbo();
    static IndexBuffer GetIbo();
    static TerrainChunk* GetChunk();
    static std::string GetTerrainMaterial();
    static void SetTerrainMaterial(const std::string& material);
  };
}

#endif //MCC_TERRAIN_H