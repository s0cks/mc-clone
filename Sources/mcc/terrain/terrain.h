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

#define FOR_EACH_TERRAIN_TEXTURE(V) \
  V(Concrete, "concrete.png")       \
  V(Wood, "wood.png")               \
  V(Planks, "planks.png")

  enum TerrainTexture {
#define DEFINE_TERRAIN_TEXTURE(Name, file) k##Name##Texture,
    FOR_EACH_TERRAIN_TEXTURE(DEFINE_TERRAIN_TEXTURE)
#undef DEFINE_TERRAIN_TEXTURE

    kNumberOfTerrainTextures,
    kDefaultTerrainTexture = kConcreteTexture,
  };

  static inline const char*
  GetTerrainTextureName(const TerrainTexture texture) {
    switch(texture) {
#define DEFINE_TOSTRING(Name, File) \
      case k##Name##Texture: return #Name;
      FOR_EACH_TERRAIN_TEXTURE(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
      default:
        return "unknown";
    }
  }

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
    static TerrainTexture GetTexture(); //TODO: remove
    static void SetTexture(const TerrainTexture texture); //TODO: remove
  };
}

#endif //MCC_TERRAIN_H