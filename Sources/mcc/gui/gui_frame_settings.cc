#include "mcc/gui/gui_frame_settings.h"
#include "mcc/renderer/renderer.h"
#include "mcc/terrain/terrain.h"

#include "mcc/window/window.h"
#include "mcc/camera/perspective_camera.h"

namespace mcc::gui {
  using renderer::Renderer;

  SettingsFrame::SettingsFrame():
    Frame("Settings", kBottom, glm::vec2(0.0f, Window::Get()->GetHeight() / 8.0f)) {
  }

  void SettingsFrame::Render(nk::Context* ctx) {
    const auto size = nk_widget_size(ctx);
    const auto material = terrain::Terrain::GetTerrainMaterialName();
    nk_layout_row_dynamic(ctx, size.y, 1);
    nk_labelf(ctx, NK_TEXT_LEFT, "Terrain Materials (%" PRIu64 "):", GetNumberOfMaterials());
    VisitAllMaterials([&](const std::string& mat) {
      const auto selected = material == mat;
      if(nk_option_label(ctx, mat.c_str(), selected) && material != mat)
        terrain::Terrain::SetTerrainMaterial(mat);
      return true;
    });
  }
}