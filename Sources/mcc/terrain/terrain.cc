#include "mcc/terrain/terrain.h"
#include "mcc/engine/engine.h"
#include "mcc/shader/shader.h"
#include "mcc/texture/texture.h"

#include "mcc/flags.h"

namespace mcc::terrain {
  static VertexArrayObject vao_(kInvalidVertexArrayObject);
  static TerrainVertexBuffer vbo_;
  static IndexBuffer ibo_;
  static Shader shader_;

  static VertexList vertices_;
  static IndexList indices_;

  static RelaxedAtomic<TerrainTexture> tex_(TerrainTexture::kDefaultTerrainTexture);
  static texture::Texture textures_[kNumberOfTerrainTextures];

  static inline texture::Texture
  GetSelectedTexture() {
    return textures_[(const TerrainTexture&) tex_];
  }

#define WIDTH (size[0])
#define HALF_WIDTH (WIDTH / 2.0f)
#define DEPTH (size[1])
#define HALF_DEPTH (DEPTH / 2.0f)
#define QUADS ((WIDTH - 1) * (DEPTH - 1))
#define INDICES (QUADS * 6)
#define VERTICES (QUADS * 4)

  static inline void
  GenerateIndices(IndexList& indices, const glm::vec2 size) {
    DLOG(INFO) << "generating " << INDICES << ".....";
    for(auto z = 0.0f; z < DEPTH - 1; z++) {
      for(auto x = 0.0f; x < WIDTH - 1; x++) {
        unsigned int btm_left = z * WIDTH + x;
        unsigned int top_left = (z + 1) * WIDTH + x;
        unsigned int top_right = (z + 1) * WIDTH + x + 1;
        unsigned int btm_right = z * WIDTH + x + 1;
        indices.push_back(btm_left);
        indices.push_back(top_left);
        indices.push_back(top_right);
        
        indices.push_back(btm_left);
        indices.push_back(top_right);
        indices.push_back(btm_right);
      }
    }
  }

  static inline void 
  GenerateVertices(VertexList& vertices, const glm::vec2 size) {
    DLOG(INFO) << "generating " << VERTICES << "....";
    for(auto z = -HALF_DEPTH; z < +HALF_DEPTH; z++) {
      for(auto x = -HALF_WIDTH; x < +HALF_WIDTH; x++) {
        const auto uvZ = static_cast<float>(x) / DEPTH; 
        const auto uvX = static_cast<float>(z) / WIDTH;
        vertices.push_back(Vertex {
          .pos = glm::vec3(x, -3.0f, z),
          .uv = glm::vec2(abs(uvX), abs(uvZ)),
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

  IndexBuffer Terrain::GetIbo() {
    return ibo_;
  }

  void Terrain::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
  }

  void Terrain::OnPreInit() {
    DLOG(INFO) << "generating terrain data....";
    const glm::vec2 size(128);
    GenerateVertices(vertices_, size);
    DLOG(INFO) << "generated " << vertices_.size() << " vertices: " << vertices_;
    GenerateIndices(indices_, size);
    DLOG(INFO) << "generated " << indices_.size() << " indices: " << indices_;
  }

  void Terrain::OnInit() {

  }

  void Terrain::OnPostInit() {
#define LOAD_TERRAIN_TEXTURE(Name, File) \
    textures_[k##Name##Texture] = texture::Texture::LoadFrom(FLAGS_resources + "/textures/" + (File));

    FOR_EACH_TERRAIN_TEXTURE(LOAD_TERRAIN_TEXTURE)
#undef LOAD_TERRAIN_TEXTURE


    shader_ = CompileShader("terrain");
    vao_ = VertexArrayObject();
    DLOG(INFO) << "terrain vao: " << vao_;

    VertexArrayObjectBindScope vao(vao_);
    vbo_ = TerrainVertexBuffer(vertices_);
    ibo_ = IndexBuffer(indices_);
  }

  void Terrain::Render(const glm::mat4 projection, const glm::mat4 view) {
    glm::mat4 model(1.0f);

    GetSelectedTexture().Bind0();
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

  void Terrain::SetTexture(const TerrainTexture texture) {
    tex_ = texture;
  }

  TerrainTexture Terrain::GetTexture() {
    return (const TerrainTexture&) tex_;
  }
}