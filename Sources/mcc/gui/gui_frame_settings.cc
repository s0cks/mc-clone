#include "mcc/gui/gui_frame_settings.h"
#include "mcc/window.h"
#include "mcc/renderer/renderer.h"
#include "mcc/terrain/terrain.h"

#include "mcc/physics/physics.h"

namespace mcc::gui {
  using renderer::Renderer;

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

    nk_layout_row_dynamic(ctx, size.y / 2, 1);
    const auto g = physics::PhysicsBehavior::GetGravity();
    float gravity = g[1];
    nk_property_float(ctx, "Gravity:", -1.0f, &gravity, 1.0f, 0.1f, 1);
    physics::PhysicsBehavior::SetGravity(glm::vec3(g[0], gravity, g[2]));
  }
}