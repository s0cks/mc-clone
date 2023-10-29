#include "mcc/renderer/renderer.h"
#include "mcc/ecs/coordinator.h"
#include "mcc/engine/engine.h"

#include "mcc/window.h"
#include "mcc/shape/square.h"
#include "mcc/shape/plane.h"
#include "mcc/camera/perspective_camera.h"

#include "mcc/gui/gui_window.h"

#include "mcc/terrain/terrain.h"

namespace mcc {
  static Tick last_;
  static RelaxedAtomic<uint64_t> frames_;
  static RelaxedAtomic<uint64_t> fps_;
  static RelaxedAtomic<Renderer::State> state_;

  static Shader lightingShader_;

  static mesh::Mesh* mesh_;
  static Shader shader_;

  void Renderer::SetState(const Renderer::State state) {
    state_ = state;
  }

  Renderer::State Renderer::GetState() {
    return (Renderer::State) state_;
  }

  void Renderer::OnPreInit() {
    Renderable::SetComponentId(Components::Register<Renderable>());
  }

  void Renderer::OnInit() {

  }

  void Renderer::PreRender() {
    SetState(Renderer::kPreRender);
    const auto mode = Mouse::IsPressed(kMouseButton1)
        ? GL_LINE
        : GL_FILL;
    glPolygonMode(GL_FRONT_AND_BACK, mode);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  }

  void Renderer::PostRender() {
    SetState(Renderer::kPostRender);
    const auto window = Window::GetHandle();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  void Renderer::OnPostInit() {
    Systems::Register<Renderer>();
    Signature sig;
    sig.set(Components::GetComponentIdForType<Renderable>());
    Systems::SetSignature<Renderer>(sig);
    Engine::OnTick(&OnTick);

    gui::Window::Init();

    Mouse::Register(MouseButton::kMouseButton1, MouseButtonState::kMousePressed, []() {
      gui::Window::SetCurrent(gui::Window::New(glm::vec2(400.0f, 400.0f)));
    });
  }

  void Renderer::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
  }

  void Renderer::RenderEntity(const glm::mat4 projection, const glm::mat4 view, const Entity e) {
    const auto& renderable = Components::GetComponent<Renderable>(e);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    const auto& shader = renderable.shader;
    const auto& texture = renderable.texture;

    // texture.Bind0();
    shader.ApplyShader();
    shader.SetMat4("model", model);
    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);
    shader.SetVec3("color", glm::vec3(0.0f, 0.0f, 1.0f));
    // shader.SetInt("texture1", 0);
    shader.ApplyShader();
    const auto& mesh = renderable.mesh;
    mesh->Render();
  }

  void Renderer::OnTick(const Tick& tick) {
    frames_ += 1;
    if(tick.dts >= (NSEC_PER_MSEC * 1)) {
      const auto diff = (tick.ts - last_.ts);
      fps_ = ((uint64_t) frames_) * (1.0 * (NSEC_PER_SEC / diff));
      last_ = tick;
      frames_ = 0;
    }

    SetState(Renderer::kRender);
    PreRender();
    {
      glEnable(GL_DEPTH_TEST);
      auto proj = camera::PerspectiveCameraBehavior::CalculateProjectionMatrix();
      auto view = camera::PerspectiveCameraBehavior::CalculateViewMatrix();
      terrain::Terrain::Render(proj, view);
      Systems::ForEachEntityInSystem<Renderer>([&](const Entity& e) {
        RenderEntity(proj, view, e);
      });
      glDisable(GL_DEPTH_TEST);
    }

    // if(gui::Window::HasCurrent()) {
    //   glDisable(GL_DEPTH_TEST);
    //   glDisable(GL_CULL_FACE);
    //   const auto size = Window::GetSize();
    //   glm::mat4 proj = glm::mat4(1.0f);
    //   proj = glm::ortho(0.0f, size[0], size[1], 0.0f, -1000.0f, 1000.0f);
    //   glm::mat4 view = glm::mat4(1.0f);
    //   view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    //   gui::Window::GetCurrent()->Render(proj, view);
    //   glEnable(GL_DEPTH_TEST);
    // }

    PostRender();
  }

  uint64_t Renderer::GetFrameCount() {
    return (uint64_t) frames_;
  }

  uint64_t Renderer::GetFPS() {
    return (uint64_t) fps_;
  }
}