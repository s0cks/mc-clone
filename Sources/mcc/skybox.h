#ifndef MCC_SKYBOX_H
#define MCC_SKYBOX_H

#include <vector>

#include "mcc/rx.h"
#include "mcc/gfx.h"

#include "mcc/shader/shader.h"
#include "mcc/shader/shader_pipeline.h"
#include "mcc/texture/texture.h"

#include "mcc/pipeline.h"

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

    static inline std::ostream&
    operator<<(std::ostream& stream, const VertexList& rhs) {
      stream << "[";
      for(auto idx = 0; idx < rhs.size(); idx++) {
        stream << rhs[idx];
        if((idx + 1) < rhs.size())
          stream << ", ";
      }
      stream << "]";
      return stream;
    }

    class VertexBuffer : public VertexBufferTemplate<Vertex, kStaticUsage> {
      enum Layout {
        kPositionOffset = offsetof(Vertex, pos),
      };

      DEFINE_VEC3F_VERTEX_BUFFER_ATTR(0, kPositionOffset, sizeof(Vertex), Position);
    public:
      explicit VertexBuffer(const BufferObjectId id = kInvalidBufferObject):
        VertexBufferTemplate(id) {  
      }
      explicit VertexBuffer(const Vertex* vertices, const uint64_t num_vertices):
        VertexBufferTemplate(vertices, num_vertices) {
        PositionAttribute::Bind();
      }
      explicit VertexBuffer(const VertexList& vertices):
        VertexBuffer(&vertices[0], vertices.size()) {  
      }
      VertexBuffer(const VertexBuffer& rhs):
        VertexBufferTemplate(rhs) {
      }
      ~VertexBuffer() override = default;
      
      void operator=(const VertexBuffer& rhs) {
        VertexBufferTemplate::operator=(rhs);
      }

      void operator=(const BufferObjectId& rhs) {
        BufferObject::operator=(rhs);
      }
    };
    DEFINE_RESOURCE_SCOPE(VertexBuffer);

    struct Skybox {
      VertexArrayObject vao;
      VertexBuffer vbo;
      TextureRef texture;
      ShaderRef shader;

      Skybox() = default;
      Skybox(const Skybox& rhs) = default;
      ~Skybox() = default;
      Skybox& operator=(const Skybox& rhs) = default;
    private:
      explicit Skybox(TextureRef texture, ShaderRef shader);
      static void OnPostInit();
      static void SetSkybox(Skybox* skybox);
    public:
      static void Init();
      static Skybox* Get();
      static Skybox* New(TextureRef texture, ShaderRef shader);
      static rx::observable<Skybox*> GetObservable();

      static inline Skybox*
      New(TextureRef texture) {
        return New(texture, GetShader("skybox"));
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
        AddChild(new ApplyPipeline([this]() {
          const auto& shader = skybox_->shader;
          shader->ApplyShader();
          shader->SetUniformBlock("Camera", 0);
          shader->SetInt("tex", 0);
        }));
        AddChild(new ApplyPipeline([this]() {
          VertexArrayObjectScope scope(skybox_->vao);
          glDrawArrays(GL_TRIANGLES, 0, skybox_->vbo.length());
          CHECK_GL(FATAL);
        }));
      }
      ~RenderSkyboxPipeline() override = default;

      void Render() override {
        if(!skybox_)
          return;

        const auto& texture = skybox_->texture;
        const auto& vao = skybox_->vao;
        InvertedCullFaceScope cull_face;
        DepthTestScope depth_test(gfx::kLequal);
        TextureBindScope<0> tex(texture);
        RenderChildren();
      }
    };
  }
}

#endif //MCC_SKYBOX_H