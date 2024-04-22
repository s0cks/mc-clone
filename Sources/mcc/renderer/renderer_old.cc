  // class RenderEntityPipeline : public Pipeline {
  // protected:
  //   const Entity entity_;
  // public:
  //   explicit RenderEntityPipeline(const Entity entity):
  //     Pipeline(),
  //     entity_(entity) {
  //   }
  //   ~RenderEntityPipeline() override = default;
  //   void Render() override {
  //     const auto renderable = Renderable::GetState(entity_);
  //     const auto transform = physics::Transform::GetState(entity_);
  //     VLOG(1) << "rendering entity " << entity_ << " w/ " << *(*renderable).data();
  //     glm::mat4 model = glm::mat4(1.0f);
  //     model = glm::translate(model, (*transform)->position);
  //     // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
  //     const auto& shader = (*renderable)->shader;
  //     const auto& texture = (*renderable)->texture;
  //     const auto& material = (*renderable)->material;
  //     glm::vec3 lightColor = glm::vec3(1.0f, 0.0f, 0.0f);
  //     glm::vec3 lightPos = glm::vec3(0.0f);
  //     AmbientLight::Visit([&lightColor,&lightPos](const Entity& l, const ComponentState<AmbientLight>& light) {
  //       lightColor = light->color;

  //       const auto lt = physics::Transform::GetState(l);
  //       LOG_IF(FATAL, !lt) << "no transform found for AmbientLight component of entity " << l;
  //       lightPos = (*lt)->position;
  //       return true;
  //     });

  //     if(texture.valid()) 
  //       texture->Bind(0);

  //     shader->ApplyShader();
  //     shader->SetMat4("model", model);
  //     const auto diffuseColor = lightColor * glm::vec3(0.5f);
  //     const auto ambientColor = diffuseColor * glm::vec3(0.8f);
  //     shader->SetLight("light", lightPos, ambientColor, diffuseColor, glm::vec3(1.0f));
  //     shader->SetInt("entity", entity_.id());
  //     shader->SetMaterial("material");
  //     shader->SetInt("tex0", 0);
  //     shader->SetVec3("lightColor", lightColor);
  //     shader->SetUniformBlock("Camera", 0);
  //     shader->ApplyShader();
  //     const auto& mesh = (*renderable)->mesh;
  //     mesh->Render();
  //     Renderer::IncrementEntityCounter();
  //     Renderer::IncrementVertexCounter(mesh->vbo().length());
  //   }
  // };

  // class RenderEntitiesPipeline : public Pipeline {
  // public:
  //   RenderEntitiesPipeline() = default;
  //   ~RenderEntitiesPipeline() override = default;

  //   void Render() override {
  //     VLOG(3) << "rendering entities....";
  //     InvertedCullFaceScope cull_face;
  //     Renderer::VisitEntities([&](const Entity& e) {
  //       RenderEntityPipeline pipe(e);
  //       pipe.Render();
  //       return true;
  //     });
  //   }
  // };

  // class RenderFbPipeline : public Pipeline {
  // private:
  //   FrameBuffer* fb_;
  //   ShaderRef shader_;
  //   BloomPipeline<> bloom_;
  //   fbuff::RenderFrameBufferPipeline pipeline_;
  // public:
  //   explicit RenderFbPipeline(FrameBuffer* fb):
  //     Pipeline(),
  //     fb_(fb),
  //     shader_(GetShader("shader:framebuffer")),
  //     bloom_(fb, Dimension(Window::Get()->GetSize()), GetShader("shader:blur")),
  //     pipeline_(fb, fbuff::kColorAndDepthClearMask) {
  //     pipeline_.AddChild(new ApplyPipeline([this]() {
  //       fb_->GetColorBufferAttachment(0)->GetTexture()->Bind(0);
  //       bloom_.GetFrameBuffer(0)->GetColorBufferAttachment(0)->GetTexture()->Bind(1);
  //     }));
  //     pipeline_.AddChild(new ApplyShaderPipeline(shader_, [](const ShaderRef& shader) {
  //       shader->SetInt("tex", 0);
  //       shader->SetInt("bloomTex", 1);
  //       shader->SetBool("bloom", true);
  //       shader->SetBool("hdr", true);
  //       shader->SetFloat("gamma", 2.2f);
  //       shader->SetFloat("exposure", 1.0f);
  //     }));
  //   }

  //   void Render() override {
  //     fb_->Unbind();
  //     bloom_.Render();
  //     pipeline_.Render();
  //   }
  // };

      // signature_.set(Renderable::GetComponentId());
    // signature_.set(physics::Transform::GetComponentId());
    //tracker_.SetSignature(signature_);
    // DLOG(INFO) << "signature: " << signature_;

    //Window::AddFrame(gui::SettingsFrame::New());
    //Window::AddFrame(gui::RendererFrame::New());


      // const auto camera = camera::PerspectiveCameraBehavior::GetCameraComponent();
      // LOG_IF(FATAL, !camera) << "no camera component found.";
      // (*camera)->ComputeMatrices();
      // const auto cam_buff = Renderer::GetCameraUniformBuffer();
      // cam_buff->Update((const camera::PerspectiveCameraData*) (*camera).data());
      // light::DirectionalLight::Visit([](const Entity& e, const ComponentState<light::DirectionalLight>& state) {
      //   light::DirectionalLight::GetBufferObject()->Update(state.data());
      //   return true;
      // });
      // light::PointLight::Visit([](const Entity& e, const ComponentState<light::PointLight>& state) {
      //   light::PointLight::GetUniformBufferObject()->Update(state.data());
      //   return true;
      // });