#include "mcc/mesh/mesh.h"
#include "mcc/engine/engine.h"

namespace mcc::mesh {
  static VertexArrayObject vao_(kInvalidVertexArrayObject);

  static inline void
  InitializeVao() {
    vao_ = VertexArrayObject();
    VertexArrayObjectBindScope scope(vao_);
  }

  void Mesh::OnPostInit() {
    InitializeVao();
  }

  void Mesh::Init() {
    Engine::OnPostInit(&OnPostInit);
  }
  
  void Mesh::Render() {
    VertexArrayObjectBindScope vao(vao_);
    glDrawArrays(GL_TRIANGLES, 0, vbo_.length());
    CHECK_GL(FATAL);
  }

  void IndexedMesh::Render() {
    VertexArrayObjectBindScope vao(vao_);
    glDrawElements(GL_TRIANGLES, ibo_.length(), ibo_.type(), 0);
    CHECK_GL(FATAL);
  }

  Mesh* NewMesh(const VertexList& vertices) {
    VertexArrayObjectBindScope vao(vao_);
    return new Mesh(vertices);
  }

  Mesh* NewMesh(const VertexList& vertices, const IndexList& indices) {
    VertexArrayObjectBindScope vao(vao_);
    return new IndexedMesh(vertices, indices);
  }

  Mesh* NewUVSphere(const uint64_t latCount, const uint64_t lonCount) {
    if(latCount < 2 || lonCount < 2) {
      DLOG(ERROR) << "cannot generate a UV Sphere w/ " << latCount << "," << lonCount;
      return nullptr;
    }

    const int total_indices = 6 * (latCount - 1) * lonCount;
    const int total_vertices = (latCount + 1) * (lonCount + 1);
    const float latStep = PI / latCount;
    const float lonStep = 2 * PI / lonCount;

    VertexList vertices;
    vertices.reserve(total_vertices);

    IndexList indices;
    indices.reserve(total_indices);

    uint64_t vertex, index;
    for(auto lat = 0; lat <= latCount; lat++) {
      for(auto lon = 0; lon <= lonCount; lon++) {
        vertices.push_back(Vertex {
          .pos = glm::vec3(
            cos(lon * lonStep) * sin(lat * latStep),
            sin(lon * lonStep) * sin(lat * latStep),
            cos(lat * latStep - PI)),
          .color = glm::vec3(1.0f, 0.0f, 0.0f),
          .normal = glm::vec3(0.0f, 0.0f, 0.0f),
          .uv = glm::vec2(static_cast<float>(lon / lonCount), static_cast<float>(lat / latCount)),
        });
      }
    }

    int v = lonCount + 1;
    for(int lon = 0; lon < lonCount; lon++) {
      indices.push_back(lon);
      indices.push_back(v);
      indices.push_back(v + 1);
      v += 1;
    }

    v = lonCount + 1;
    for(int lat = 1; lat < latCount - 1; lat++) {
      for(int lon = 0; lon < lonCount; lon++) {
        indices.push_back(v);
        indices.push_back(v + lonCount + 1);
        indices.push_back(v + 1);
        indices.push_back(v + 1);
        indices.push_back(v + lonCount + 1);
        indices.push_back(v + lonCount + 2);

        v += 1;
      }

      v += 1;
    }

    for(int lon = 0; lon < lonCount; lon++) {
      indices.push_back(v);
      indices.push_back(v + lonCount + 1);
      indices.push_back(v + 1);
      v += 1;
    }

    return NewMesh(vertices, indices);
  }
}