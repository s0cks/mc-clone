#include "mcc/renderer/renderer.h"
#include "mcc/ecs/coordinator.h"
#include "mcc/engine/engine.h"

#include "mcc/thread_local.h"

#include "mcc/window.h"
#include "mcc/camera/perspective_camera.h"

#include "mcc/font/font.h"
#include "mcc/font/font_renderer.h"

#include "mcc/terrain/terrain.h"

#include "mcc/gui/gui.h"
#include "mcc/gui/gui_frame.h"
#include "mcc/gui/gui_frame_settings.h"
#include "mcc/gui/gui_frame_renderer.h"

#include "mcc/renderer/stage.h"
#include "mcc/physics/transform.h"
#include "mcc/physics/rigid_body.h"
#include "mcc/lighting/ambient_light.h"

namespace mcc::renderer {
  static ThreadLocal<uv_loop_t> loop_;
  static ThreadLocal<PreRenderStage> pre_render_;
  static ThreadLocal<RenderTerrainStage> render_terrain_;
  static ThreadLocal<RenderEntitiesStage> render_entities_;
  static ThreadLocal<RenderGuiStage> render_gui_;
  static ThreadLocal<RenderScreenStage> draw_gui_;
  static ThreadLocal<PostRenderStage> post_render_;
  static ThreadLocal<FrameBuffer> frame_buffer_;

  static Tick last_;
  static RelaxedAtomic<uint64_t> frames_;
  static RelaxedAtomic<uint64_t> fps_;
  static RelaxedAtomic<RendererState> state_;
  static RelaxedAtomic<uint64_t> entities_(0);
  static uint64_t frame_start_ns_;
  static uint64_t last_frame_ns_;

  static mesh::Mesh* mesh_;
  static Shader shader_;

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
  }

  void Renderer::OnPreInit() {
  }

  void Renderer::OnInit() {
    const auto loop = uv_loop_new();
    post_render_.Set(new PostRenderStage(loop));
    draw_gui_.Set(new RenderScreenStage(loop));
    render_gui_.Set(new RenderGuiStage(loop));
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

    Engine::OnTick(&OnTick);

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

  void Renderer::OnTick(const Tick& tick) {
    frames_ += 1;
    if(tick.dts >= (NSEC_PER_MSEC * 1)) {
      const auto diff = (tick.ts - last_.ts);
      fps_ = ((uint64_t) frames_) * (1.0 * (NSEC_PER_SEC / diff));
      last_ = tick;
      frames_ = 0;
    }

    Run();
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

  void Renderer::Run(const uv_run_mode mode) {
    VLOG(1) << "running....";
    frame_start_ns_ = uv_hrtime();
    uv_run(GetLoop(), mode);
    last_frame_ns_ = (uv_hrtime() - frame_start_ns_);
    VLOG(1) << "done in " << (last_frame_ns_ / NSEC_PER_MSEC) << "ms.";
    samples_ << RendererSample {
      .duration = last_frame_ns_,
      .entities = (uint64_t) entities_,
    };
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
}