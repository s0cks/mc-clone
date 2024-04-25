#ifndef MCC_SKYBOX_H
#define MCC_SKYBOX_H

#include <vector>

#include "mcc/rx.h"
#include "mcc/gfx.h"
#include "mcc/vao/vao.h"
#include "mcc/vbo/vbo.h"

#include "mcc/program/program.h"
#include "mcc/texture/texture.h"

#include "mcc/pipeline.h"

#include "mcc/vao/vao_scope.h"

namespace mcc {
  namespace skybox {
    struct Vertex {
      glm::vec3 pos;

      friend std::ostream& operator<<(std::ostream& stream, const Vertex& rhs) {
        stream << "skybox::Vertex(";
        stream << "pos=" << glm::to_string(rhs.pos);
        stream << ")";
        return stream;
      }
    };

    typedef std::vector<Vertex> VertexList;

    struct Skybox {
      Vao* vao;
      Vbo* vbo;
      TextureRef texture;
      ProgramRef shader;

      Skybox() = default;
      Skybox(const Skybox& rhs) = default;
      ~Skybox() = default;
      Skybox& operator=(const Skybox& rhs) = default;
    private:
      explicit Skybox(TextureRef texture, ProgramRef shader);
      static void OnPostInit();
      static void SetSkybox(Skybox* skybox);
    public:
      static void Init();
      static Skybox* Get();
      static Skybox* New(TextureRef texture, ProgramRef shader);
      static rx::observable<Skybox*> GetObservable();

      static inline Skybox*
      New(TextureRef texture) {
        return New(texture, Program::New("skybox"));
      }
    };

    class RenderSkyboxPipeline : public Pipeline {
    private:
      Skybox* skybox_;
    public:
      explicit RenderSkyboxPipeline(Skybox* skybox = Skybox::Get()):
        Pipeline(),
        skybox_(skybox) {
        Skybox::GetObservable()
          .subscribe([this](Skybox* skybox) {
            skybox_ = skybox;
          });
        // AddChild(new ApplyPipeline([this]() {
        //   const auto& shader = skybox_->shader;
        //   shader->ApplyShader();
        //   shader->SetUniformBlock("Camera", 0);
        //   shader->SetInt("tex", 0);
        // }));
        AddChild(new ApplyPipeline([this]() {
          VaoBindScope scope(skybox_->vao);
          glDrawArrays(GL_TRIANGLES, 0, skybox_->vbo->GetLength());
          CHECK_GL(FATAL);
        }));
      }
      ~RenderSkyboxPipeline() override = default;

      void Render() override;
    };
  }
}

#endif //MCC_SKYBOX_H