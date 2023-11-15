#include "mcc/renderer/renderer.h"
#include "mcc/engine/engine.h"

#include "mcc/thread_local.h"

#include "mcc/window.h"
#include "mcc/camera/perspective_camera.h"

#include "mcc/font/font.h"
#include "mcc/font/font_renderer.h"

#include "mcc/terrain/terrain.h"

#include "mcc/light/point.h"
#include "mcc/light/directional.h"

#include "mcc/gui/gui.h"
#include "mcc/gui/gui_frame.h"
#include "mcc/gui/gui_frame_settings.h"
#include "mcc/gui/gui_frame_renderer.h"

#include "mcc/renderer/stage.h"
#include "mcc/physics/transform.h"
#include "mcc/physics/rigid_body.h"
#include "mcc/lighting/ambient_light.h"

#include "mcc/skybox.h"

namespace mcc::renderer {
  static ThreadLocal<uv_loop_t> loop_;
  static ThreadLocal<PreRenderStage> pre_render_;
  static ThreadLocal<RenderTerrainStage> render_terrain_;
  static ThreadLocal<RenderEntitiesStage> render_entities_;
  static ThreadLocal<RenderGuiStage> render_gui_;
  static ThreadLocal<RenderScreenStage> draw_gui_;
  static ThreadLocal<PostRenderStage> post_render_;
  static ThreadLocal<FrameBuffer> frame_buffer_;
  static ThreadLocal<camera::PerspectiveCameraDataUniformBufferObject> cam_data_;

  static RelaxedAtomic<RendererState> state_;
  static uint64_t frame_start_ns_;
  static uint64_t last_frame_ns_;
  static uint64_t frame_end_ns_;
  static uint64_t frame_dts_;
  static uint64_t last_second_;
  static RelaxedAtomic<uint64_t> frames_;
  static RelaxedAtomic<uint64_t> fps_;
  static RelaxedAtomic<uint64_t> entities_;
  static RelaxedAtomic<uint64_t> vertices_;

  static mesh::Mesh* mesh_;
  static shader::Shader shader_;

  static Signature signature_;
  static EntitySet tracked_;

  static RelaxedAtomic<Renderer::Mode> mode_(Renderer::kDefaultMode);
  static RendererSampleSeries samples_;

  void Renderer::SetMode(const Renderer::Mode mode) {
    mode_ = mode;
  }

  Renderer::Mode Renderer::GetMode() {
    return (Renderer::Mode) mode_;
  }

  void Renderer::SetState(const RendererState state) {
    state_ = state;
  }

  RendererState Renderer::GetState() {
    return (RendererState) state_;
  }
 
  void Renderer::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
    FrameBuffer::Init();
    Renderable::Init();
    AmbientLight::Init();
    physics::Transform::Init();
    physics::RigidBody::Init();
    light::DirectionalLight::Init();
    light::PointLight::Init();
    skybox::Skybox::Init();
  }

  void Renderer::OnPreInit() {

  }

  void Renderer::OnInit() {
    const auto loop = uv_loop_new();
    post_render_.Set(new PostRenderStage(loop));
    render_gui_.Set(new RenderGuiStage(loop));
    draw_gui_.Set(new RenderScreenStage(loop));
    render_entities_.Set(new RenderEntitiesStage(loop));
    render_terrain_.Set(new RenderTerrainStage(loop));
    pre_render_.Set(new PreRenderStage(loop));
    SetLoop(loop);
  }

  void Renderer::OnPostInit() {
    signature_.set(Renderable::GetComponentId());
    signature_.set(physics::Transform::GetComponentId());
    DLOG(INFO) << "signature: " << signature_;

    Window::AddFrame(gui::SettingsFrame::New());
    Window::AddFrame(gui::RendererFrame::New());

    const auto size = Window::GetSize();
    frame_buffer_.Set(FrameBuffer::New(size[0], size[1]));
    cam_data_.Set(new camera::PerspectiveCameraDataUniformBufferObject());

    Entity::OnSignatureChanged()
      .subscribe([](EntitySignatureChangedEvent* e) {
        const auto& esig = e->signature;
        const auto& eid = e->id;
        if((esig & signature_) == signature_) {
          tracked_.insert(eid);
        } else {
          tracked_.erase(eid);
        }
      });
    Entity::OnDestroyed()
      .subscribe([](EntityDestroyedEvent* e) {
        tracked_.erase(e->id);
      });
  }

  uint64_t Renderer::GetFrameCount() {
    return (uint64_t) frames_;
  }

  uint64_t Renderer::GetFPS() {
    return (uint64_t) fps_;
  }

  uint64_t Renderer::GetLastFrameTimeInNanoseconds() {
    return last_frame_ns_;
  }

  void Renderer::SetLoop(uv_loop_t* loop) {
    loop_.Set(loop);
  }

  uv_loop_t* Renderer::GetLoop() {
    return loop_.Get();
  }

  static constexpr const auto kTargetFramesPerSecond = 60.0f;
  static constexpr const float kRate = NSEC_PER_SEC / (kTargetFramesPerSecond * NSEC_PER_SEC);

  void Renderer::Run(const uv_run_mode mode) {
    const auto start_ns = uv_hrtime();
    const auto delta_ns = (start_ns - last_frame_ns_);
    if((delta_ns / (NSEC_PER_SEC * 1.0f)) < kRate) {
      return;
    }
    frame_start_ns_ = start_ns;
    frame_dts_ = delta_ns;
    if((start_ns - last_second_) >= NSEC_PER_SEC) {
      fps_ = frames_;
      frames_ = 0;
      last_second_ = frame_start_ns_;
    }
    uv_run(GetLoop(), mode);
    frame_end_ns_ = uv_hrtime();
    const auto total_ns = (frame_end_ns_ - frame_start_ns_);
    samples_ << RendererSample {
      .duration = total_ns,
      .entities = (uint64_t) entities_,
    };
    frames_ += 1;
    last_frame_ns_ = frame_start_ns_;
  }

  RendererSampleSeries* Renderer::GetSamples() {
    return &samples_;
  }

  FrameBuffer* Renderer::GetFrameBuffer() {
    return frame_buffer_.Get();
  }

  void Renderer::IncrementEntityCounter(const uint64_t value) {
    entities_ += value;
  }

  void Renderer::DecrementEntityCounter(const uint64_t value) {
    entities_ -= value;
  }

  void Renderer::SetEntityCounter(const uint64_t value) {
    entities_ = value;
  }

  void Renderer::IncrementVertexCounter(const uint64_t value) {
    vertices_ += value;
  }

  void Renderer::DecrementVertexCounter(const uint64_t value) {
    vertices_ -= value;
  }

  void Renderer::SetVertexCounter(const uint64_t value) {
    vertices_ = value;
  }

  uint64_t Renderer::GetVertexCounter() {
    return (uint64_t) vertices_;
  }

  uint64_t Renderer::GetEntityCounter() {
    return (uint64_t) entities_;
  }

  Signature Renderer::GetSignature() {
    return signature_;
  }

  bool Renderer::VisitEntities(std::function<bool(const Entity&)> callback) {
    for(auto& e : tracked_) {
      if(!callback(e))
        return false;
    }
    return true;
  }

  camera::PerspectiveCameraDataUniformBufferObject* Renderer::GetCameraUniformBuffer() {
    return cam_data_.Get();
  }
}