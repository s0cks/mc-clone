#include "mcc/renderer/renderer.h"
#include "mcc/engine/engine.h"

#include "mcc/thread_local.h"

#include "mcc/window/window.h"
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

#include "mcc/physics/transform.h"
#include "mcc/physics/rigid_body.h"
#include "mcc/lighting/ambient_light.h"

#include "mcc/skybox.h"
#include "mcc/bloom.h"

#include "mcc/gui/shape.h"

namespace mcc::renderer {
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
  static ThreadLocal<Pipeline> pipeline_;

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
    const auto engine = engine::Engine::GetEngine();
    engine->OnPreInitEvent().subscribe(&OnPreInit);
    engine->OnInitEvent().subscribe(&OnInit);
    engine->OnPostInitEvent().subscribe(&OnPostInit);
    FrameBuffer::Init();
    Renderable::Init();
    AmbientLight::Init();
    physics::Transform::Init();
    physics::RigidBody::Init();
    light::DirectionalLight::Init();
    light::PointLight::Init();
    skybox::Skybox::Init();
  }

  void Renderer::OnPreInit(engine::PreInitEvent* e) {

  }

  void Renderer::OnInit(engine::InitEvent* e) {
    const auto size = Window::Get()->handle();
  }

  class RenderEntityPipeline : public Pipeline {
  protected:
    const Entity entity_;
  public:
    explicit RenderEntityPipeline(const Entity entity):
      Pipeline(),
      entity_(entity) {
    }
    ~RenderEntityPipeline() override = default;

    void Render() override {
      const auto renderable = Renderable::GetState(entity_);
      const auto transform = physics::Transform::GetState(entity_);
      VLOG(1) << "rendering entity " << entity_ << " w/ " << *(*renderable).data();
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, (*transform)->position);
      // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
      const auto& shader = (*renderable)->shader;
      const auto& texture = (*renderable)->texture;
      const auto& material = (*renderable)->material;

      glm::vec3 lightColor = glm::vec3(1.0f, 0.0f, 0.0f);
      glm::vec3 lightPos = glm::vec3(0.0f);
      AmbientLight::Visit([&lightColor,&lightPos](const Entity& l, const ComponentState<AmbientLight>& light) {
        lightColor = light->color;

        const auto lt = physics::Transform::GetState(l);
        LOG_IF(FATAL, !lt) << "no transform found for AmbientLight component of entity " << l;
        lightPos = (*lt)->position;
        return true;
      });

      if(texture.valid()) 
        texture->Bind(0);

      shader->ApplyShader();
      shader->SetMat4("model", model);
      const auto diffuseColor = lightColor * glm::vec3(0.5f);
      const auto ambientColor = diffuseColor * glm::vec3(0.8f);
      shader->SetLight("light", lightPos, ambientColor, diffuseColor, glm::vec3(1.0f));
      shader->SetInt("entity", entity_.id());
      shader->SetMaterial("material");
      shader->SetInt("tex0", 0);
      shader->SetVec3("lightColor", lightColor);
      shader->SetUniformBlock("Camera", 0);
      shader->ApplyShader();
      const auto& mesh = (*renderable)->mesh;
      mesh->Render();
      Renderer::IncrementEntityCounter();
      Renderer::IncrementVertexCounter(mesh->vbo().length());
    }
  };

  class RenderEntitiesPipeline : public Pipeline {
  public:
    RenderEntitiesPipeline() = default;
    ~RenderEntitiesPipeline() override = default;

    void Render() override {
      VLOG(3) << "rendering entities....";
      InvertedCullFaceScope cull_face;
      Renderer::VisitEntities([&](const Entity& e) {
        RenderEntityPipeline pipe(e);
        pipe.Render();
        return true;
      });
    }
  };

  class RenderFbPipeline : public Pipeline {
  private:
    FrameBuffer* fb_;
    ShaderRef shader_;
    BloomPipeline<> bloom_;
    RenderFrameBufferPipeline pipeline_;
  public:
    explicit RenderFbPipeline(FrameBuffer* fb):
      Pipeline(),
      fb_(fb),
      shader_(GetShader("shader:framebuffer")),
      bloom_(fb, Dimension(Window::Get()->GetSize()), GetShader("shader:blur")),
      pipeline_(fb, kColorAndDepthClearMask) {
      pipeline_.AddChild(new ApplyPipeline([this]() {
        fb_->GetColorBufferAttachment(0)->GetTexture()->Bind(0);
        bloom_.GetFrameBuffer(0)->GetColorBufferAttachment(0)->GetTexture()->Bind(1);
      }));
      pipeline_.AddChild(new ApplyShaderPipeline(shader_, [](const ShaderRef& shader) {
        shader->SetInt("tex", 0);
        shader->SetInt("bloomTex", 1);
        shader->SetBool("bloom", true);
        shader->SetBool("hdr", true);
        shader->SetFloat("gamma", 2.2f);
        shader->SetFloat("exposure", 1.0f);
      }));
    }

    void Render() override {
      fb_->Unbind();
      bloom_.Render();
      pipeline_.Render();
    }
  };

  static const d2::VertexList kTriangleVertices = {
    d2::Vertex {
      .pos = { 0.0f, 0.0f },
      .color = { 0, 0, 0, 255 },
    },
    {
      .pos = { 0.0f, 1.0f },
      .color = { 0, 0, 0, 255 },
    },
    {
      .pos = { 1.0f, 1.0f },
      .color = { 0, 0, 0, 255 },
    }
  };

  class RenderTrianglePipeline : public Pipeline {
  protected:
    d2::Mesh* mesh_;

    inline d2::Mesh* mesh() const {
      return mesh_;
    }
  public:
    RenderTrianglePipeline(const glm::mat4& projection):
      Pipeline(),
      mesh_(d2::NewMesh(kTriangleVertices)) {
      AddChild(new ApplyShaderPipeline(GetShader("shader:colored_2d"), [projection](const ShaderRef& shader) {
        shader->ApplyShader();
        shader->SetMat4("projection", projection);
        shader->ApplyShader();
      }));
      AddChild(new ApplyPipeline([this]() {
        mesh()->Draw();
      }));
    }
    ~RenderTrianglePipeline() override = default;

    void Render() override {
      DLOG(INFO) << "drawing triangle....";
      InvertedCullFaceScope cull_face;
      RenderChildren();
    }
  };

  class RendererPipeline : public Pipeline {
  private:
  public:
    RendererPipeline():
      Pipeline() {
      const auto size = Window::Get()->GetSize();
      DLOG(INFO) << "size: " << glm::to_string(size);
      const auto aspect = ((size[0] * 1.0f) / (size[1] * 1.0f));
      const auto projection = glm::ortho(0.0f, size[0] * 1.0f, size[1] * 1.0f, 0.0f, -1000.0f, 1000.0f);
      AddChild(new RenderTrianglePipeline(projection));
    }
    ~RendererPipeline() override = default;

    void Render() override {
      gui::Screen::NewFrame();
      const auto window = Window::Get()->handle();
      int width;
      int height;
      glfwGetFramebufferSize(window, &width, &height);
      CHECK_GL(FATAL);
      glViewport(0, 0, width, height);
      CHECK_GL(FATAL);
      glfwPollEvents();
      CHECK_GL(FATAL);
      //TODO: remove chunk
      glPolygonMode(GL_FRONT_AND_BACK, Renderer::GetMode());
      CHECK_GL(FATAL);
      glEnable(GL_DEPTH_TEST);
      CHECK_GL(FATAL);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      CHECK_GL(FATAL);
      glDepthFunc(GL_LEQUAL);
      CHECK_GL(FATAL);
      glEnable(GL_CULL_FACE);
      CHECK_GL(FATAL);
      glFrontFace(GL_CW);
      CHECK_GL(FATAL);
      glCullFace(GL_BACK);
      CHECK_GL(FATAL);
      glClearColor(0.4, 0.3, 0.4, 1.0f);
      CHECK_GL(FATAL);

      const auto camera = camera::PerspectiveCameraBehavior::GetCameraComponent();
      LOG_IF(FATAL, !camera) << "no camera component found.";
      (*camera)->ComputeMatrices();
      const auto cam_buff = Renderer::GetCameraUniformBuffer();
      cam_buff->Update((const camera::PerspectiveCameraData*) (*camera).data());
      light::DirectionalLight::Visit([](const Entity& e, const ComponentState<light::DirectionalLight>& state) {
        light::DirectionalLight::GetBufferObject()->Update(state.data());
        return true;
      });
      light::PointLight::Visit([](const Entity& e, const ComponentState<light::PointLight>& state) {
        light::PointLight::GetUniformBufferObject()->Update(state.data());
        return true;
      });

      RenderChildren();

      glfwSwapBuffers(window);
      CHECK_GL(FATAL);
      Renderer::ResetEntityCounter();
      Renderer::ResetVertexCounter();
    }
  };

  void Renderer::OnPostInit(engine::PostInitEvent* e) {
    signature_.set(Renderable::GetComponentId());
    signature_.set(physics::Transform::GetComponentId());
    DLOG(INFO) << "signature: " << signature_;

    //Window::AddFrame(gui::SettingsFrame::New());
    //Window::AddFrame(gui::RendererFrame::New());

    pipeline_.Set(new RendererPipeline());
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

    pipeline_.Get()->Render();

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