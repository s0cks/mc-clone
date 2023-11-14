#include "mcc/terrain/terrain_chunk.h"
#include "mcc/engine/engine.h"
#include "mcc/terrain/terrain.h"
#include "mcc/terrain/terrain_flags.h"

namespace mcc::terrain {
  static VertexArrayObject vao_(kInvalidVertexArrayObject);

  TerrainChunk* TerrainChunk::New(const VertexList& vertices, const IndexList& indices) {
    VertexArrayObjectScope scope(vao_);
    return new TerrainChunk(vertices, indices);
  }

  void TerrainChunk::OnPreInit() {

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
          .pos = glm::vec3(x, 0.0f, z),
          .uv = glm::vec2(abs(uvX), abs(uvZ)),
          .color = glm::vec3(1.0f, 1.0f, 1.0f),
        });
      }
    }
  }

  void TerrainChunk::OnPostInit() {
    DLOG(INFO) << "generating terrain data....";
    const auto size = glm::vec2(static_cast<float>(FLAGS_terrain_size));

    VertexList vertices;
    VLOG(1) << "generating terrain vertices....";
    GenerateVertices(vertices, size);
    VLOG(1) << "generated " << vertices.size() << " vertices.";
    VLOG(10) << "terrain vertices: " << vertices;

    IndexList indices;
    VLOG(1) << "generating terrain indices....";
    GenerateIndices(indices, size);
    VLOG(1) << "generated " << indices.size() << " indices.";
    VLOG(10) << "terrain indices: " <<  indices;

    vao_ = VertexArrayObject();
    Terrain::SetChunk(TerrainChunk::New(vertices, indices));
  }

  void TerrainChunk::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnPostInit(&OnPostInit);
  }

  void TerrainChunk::Render() {
    VertexArrayObjectScope vao(vao_);
    vbo_.Bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 0);
    CHECK_GL(FATAL);
    glEnableVertexAttribArray(0);
    CHECK_GL(FATAL);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, uv));
    CHECK_GL(FATAL);
    glEnableVertexAttribArray(1);
    CHECK_GL(FATAL);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, color));
    CHECK_GL(FATAL);
    glEnableVertexAttribArray(2);
    CHECK_GL(FATAL);

    ibo_.Bind();
    glDrawElements(GL_TRIANGLES, ibo_.length(), ibo_.type(), (const GLvoid*) 0);
    CHECK_GL(FATAL);
    ibo_.Unbind();
    vbo_.Unbind();
  }
}