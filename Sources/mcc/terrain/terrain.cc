#include "mcc/terrain/terrain.h"
#include "mcc/engine/engine.h"
#include "mcc/shader/shader.h"
#include "mcc/texture/texture.h"

#include "mcc/flags.h"

namespace mcc::terrain {
  static VertexArrayObject vao_(kInvalidVertexArrayObject);
  static TerrainVertexBuffer vbo_(kInvalidVertexBufferObject);
  static TerrainIndexBuffer ibo_(kInvalidIndexBufferObject);
  static Shader shader_;
  static texture::Texture texture_(texture::kInvalidTextureId);

  static VertexList vertices_;
  static IndexList indices_;

  static inline void
  GenerateIndices(IndexList& indices, const uint32_t num_vertices) {
    const uint32_t num_indices = (num_vertices - 1) * (num_vertices - 1) * 6;
    DLOG(INFO) << "generating " << num_indices << " indices for " << num_vertices << " vertices....";
    for(uint32_t col = 0; col < num_vertices - 1; col++) {
      for(uint32_t row = 0; row < num_vertices - 1; row++) {
        uint32_t top_left = (row * num_vertices) + col;
        uint32_t top_right = (top_left + 1);
        uint32_t bottom_left = ((row + 1) * num_vertices) + col;
        uint32_t bottom_right = bottom_left + 1;

        indices.push_back(top_left);
        indices.push_back(bottom_left);
        indices.push_back(bottom_right);
        indices.push_back(top_left);
        indices.push_back(bottom_right);
        indices.push_back(top_right);
      }
    }
  }

  static inline void 
  GenerateVertices(VertexList& vertices, const uint32_t width, const uint32_t length) {
    DLOG(INFO) << "generating vertices for " << width << "x" << length << " terrain....";
    for(auto z = 0; z < length; z++) {
      for(auto x = 0; x < width; x++) {
        const auto uvZ = z / static_cast<float>(length);
        const auto uvX = x / static_cast<float>(width);
        vertices.push_back(Vertex {
          .pos = glm::vec3(x * 1.0f, 1.0f, z * 1.0f),
          .uv = glm::vec2(uvX, uvZ),
          .color = glm::vec3(1.0f, 1.0f, 1.0f),
        });
      }
    }
  }

  VertexArrayObject Terrain::GetVao() {
    return vao_;
  }

  TerrainVertexBuffer Terrain::GetVbo() {
    return vbo_;
  }

  TerrainIndexBuffer Terrain::GetIbo() {
    return ibo_;
  }

  void Terrain::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
  }

  void Terrain::OnPreInit() {
    DLOG(INFO) << "generating terrain data....";
    GenerateVertices(vertices_, 10, 10);
    DLOG(INFO) << "generated " << vertices_.size() << " vertices: " << vertices_;
    GenerateIndices(indices_, 10);
    DLOG(INFO) << "generated " << indices_.size() << " indices: " << indices_;
  }

  void Terrain::OnInit() {

  }

  void Terrain::OnPostInit() {
    texture_ = texture::Texture::LoadFrom(FLAGS_resources + "/textures/concrete.png");
    shader_ = CompileShader("terrain");
    vao_ = VertexArrayObject();

    VertexArrayObjectBindScope vao(vao_);
    vbo_ = TerrainVertexBuffer(vertices_);
    ibo_ = TerrainIndexBuffer(indices_);
  }

  void Terrain::Render(const glm::mat4 projection, const glm::mat4 view) {
    glm::mat4 model(1.0f);

    texture_.Bind0();
    shader_.ApplyShader();
    shader_.SetMat4("projection", projection);
    shader_.SetMat4("view", view);
    shader_.SetMat4("model", model);
    glUniform1i(glGetUniformLocation(shader_.GetShaderId(), "tex0"), 0);
    shader_.ApplyShader();

    VertexArrayObjectBindScope scope(vao_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, (const GLvoid*) 0);
    CHECK_GL(FATAL);
  }
}