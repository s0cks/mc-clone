#include "mcc/renderer/renderer.h"
#include "mcc/ecs/coordinator.h"
#include "mcc/engine/engine.h"

#include "mcc/window.h"
#include "mcc/shape/square.h"
#include "mcc/camera/perspective_camera.h"

#include "mcc/font/font.h"
#include "mcc/font/font_renderer.h"

#include "mcc/terrain/terrain.h"

#include "mcc/gui/gui.h"

namespace mcc {
  static Tick last_;
  static RelaxedAtomic<uint64_t> frames_;
  static RelaxedAtomic<uint64_t> fps_;
  static RelaxedAtomic<Renderer::State> state_;

  static font::Font* font_ = nullptr;
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

  static inline std::string
  GetFps() {
    std::stringstream ss;
    ss << "FPS: " << (uint64_t) fps_;
    return ss.str();
  }

  void Renderer::PreRender() {
    DLOG(INFO) << "pre-render.";
    SetState(Renderer::kPreRender);
    int width;
    int height;
    glfwGetFramebufferSize(Window::GetHandle(), &width, &height);
    CHECK_GL(FATAL);
    glViewport(0, 0, width, height);
    CHECK_GL(FATAL);

    glfwPollEvents();
    CHECK_GL(FATAL);
    gui::Screen::NewFrame();
    gui::Screen::TestScreen();
  }

  void Renderer::PostRender() {
    DLOG(INFO) << "post-render.";
    SetState(Renderer::kPostRender);
    const auto window = Window::GetHandle();
    glfwSwapBuffers(window);
    CHECK_GL(FATAL);
  }

  void Renderer::OnPostInit() {
    Systems::Register<Renderer>();
    Signature sig;
    sig.set(Components::GetComponentIdForType<Renderable>());
    Systems::SetSignature<Renderer>(sig);
    Engine::OnTick(&OnTick);
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
      const auto mode = Mouse::IsPressed(kMouseButton1)
          ? GL_LINE
          : GL_FILL;
      glPolygonMode(GL_FRONT_AND_BACK, mode);
      CHECK_GL(FATAL);
      glClearColor(0.4, 0.3, 0.4, 1.0f);
      CHECK_GL(FATAL);
      glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
      CHECK_GL(FATAL);
      glEnable(GL_DEPTH_TEST);
      CHECK_GL(FATAL);
      auto proj = camera::PerspectiveCameraBehavior::CalculateProjectionMatrix();
      auto view = camera::PerspectiveCameraBehavior::CalculateViewMatrix();
      terrain::Terrain::Render(proj, view);
      Systems::ForEachEntityInSystem<Renderer>([&](const Entity& e) {
        RenderEntity(proj, view, e);
      });
      glDisable(GL_DEPTH_TEST);
      CHECK_GL(FATAL);
    }

    {
      glm::mat4 projection(1.0f);
      gui::Screen::RenderScreen(projection);
    }
    PostRender();
  }

  uint64_t Renderer::GetFrameCount() {
    return (uint64_t) frames_;
  }

  uint64_t Renderer::GetFPS() {
    return (uint64_t) fps_;
  }
}