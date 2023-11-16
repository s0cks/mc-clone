#include "mcc/gui/gui_frame_settings.h"
#include "mcc/window.h"
#include "mcc/renderer/renderer.h"
#include "mcc/terrain/terrain.h"

#include "mcc/physics/physics.h"
#include "mcc/camera/perspective_camera.h"

namespace mcc::gui {
  using renderer::Renderer;

  SettingsFrame::SettingsFrame():
    Frame("Settings", kBottom, glm::vec2(0.0f, Window::GetHeight() / 8.0f)) {
  }

  void SettingsFrame::Render(nk::Context* ctx) {
    struct nk_vec2 size = nk_widget_size(ctx);

    static const std::vector<std::string> materials = {
      "emerald",
      "jade",
      "obsidian",
    };
    nk_layout_row_dynamic(ctx, size.y / 3, materials.size() + 1);
    nk_label(ctx, "Terrain Material:", NK_TEXT_LEFT);
    for(auto idx = 0; idx < materials.size(); idx++) {
      if(nk_option_label(ctx, materials[idx].c_str(), materials[idx] == terrain::Terrain::GetTerrainMaterial()))
        terrain::Terrain::SetTerrainMaterial(materials[idx]);
    }

    nk_layout_row_dynamic(ctx, size.y / 3, 3);
    const auto g = physics::PhysicsSimulator::GetGravity();
    float gravity = g[1];
    nk_property_float(ctx, "Gravity:", -1.0f, &gravity, 1.0f, 0.1f, 1);
    physics::PhysicsSimulator::SetGravity(glm::vec3(g[0], gravity, g[2]));

    auto camera = camera::PerspectiveCameraBehavior::GetCameraComponent();
    if(camera)
      nk_property_float(ctx, "Camera Speed:", -1.0f, &camera.value()->speed, 2.0f, 0.05f, 1);
  }
}