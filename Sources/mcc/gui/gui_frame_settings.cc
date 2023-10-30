#include "mcc/gui/gui_frame_settings.h"
#include "mcc/window.h"
#include "mcc/renderer/renderer.h"
#include "mcc/terrain/terrain.h"

namespace mcc::gui {
  SettingsFrame::SettingsFrame():
    Frame("Settings", kBottom, glm::vec2(0.0f, Window::GetHeight() / 8.0f)) {
  }

  void SettingsFrame::Render(nk::Context* ctx) {
    struct nk_vec2 size = nk_widget_size(ctx);

    nk_layout_row_dynamic(ctx, size.y / 2, terrain::kNumberOfTerrainTextures + 1);
    nk_label(ctx, "Terrain: ", NK_TEXT_LEFT);
    for(auto idx = 0; idx < terrain::kNumberOfTerrainTextures; idx++) {
      const auto tex = static_cast<terrain::TerrainTexture>(idx);
      if(nk_option_label(ctx, terrain::GetTerrainTextureName(tex), terrain::Terrain::GetTexture() == tex))
        terrain::Terrain::SetTexture(tex);
    }

    nk_layout_row_dynamic(ctx, size.y / 2, 3);
    nk_label(ctx, "Render Mode: ", NK_TEXT_LEFT);
    if(nk_option_label(ctx, "Default", Renderer::GetMode() == Renderer::kDefaultMode))
      Renderer::SetMode(Renderer::kDefaultMode);
    if(nk_option_label(ctx, "Wireframe", Renderer::GetMode() == Renderer::kWireframeMode))
      Renderer::SetMode(Renderer::kWireframeMode);
  }
}