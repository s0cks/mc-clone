#include <unordered_map>

#include "mcc/mesh/mesh.h"
#include "mcc/engine/engine.h"

namespace mcc::mesh {
#define FOR_EACH_BUILTIN_MESH(V) \
  V(UVSphere)                    \
  V(Icosphere)                   \
  V(Cube)                        \
  V(PlaneXZ)                     \
  V(PlaneXY)

  enum BuiltinVao {
#define DEFINE_BUILTIN_VAO(Name) k##Name##Vao,
    FOR_EACH_BUILTIN_MESH(DEFINE_BUILTIN_VAO)
#undef DEFINE_BUILTIN_VAO
    kTotalNumberOfBuiltinVaos,
  };

  static inline std::ostream&
  operator<<(std::ostream& stream, const BuiltinVao& rhs) {
    switch(rhs) {
#define DEFINE_TOSTRING(Name) \
      case k##Name##Vao: return stream << #Name;

      FOR_EACH_BUILTIN_MESH(DEFINE_TOSTRING)
      default: return stream << "unknown";
#undef DEFINE_TOSTRING
    }
  }

  static VertexArrayObject* kBuiltinVaos = nullptr;

  void Mesh::InitializeBuiltinVaos() {
    DLOG(INFO) << "initializing builtin vaos....";
    VertexArrayObject::GenerateBatch(kTotalNumberOfBuiltinVaos, &kBuiltinVaos);
    for(auto idx = 0; idx < kTotalNumberOfBuiltinVaos; idx++) {
      DLOG(INFO) << static_cast<BuiltinVao>(idx) << " := " << kBuiltinVaos[idx];
    }
  }

#define BUILTIN_VAO(Name) kBuiltinVaos[k##Name##Vao]

  void Mesh::OnPostInit() {
    InitializeBuiltinVaos();
  }

  void Mesh::Init() {
    Engine::OnPostInit(&OnPostInit);
  }
  
  void Mesh::Render() {
    VertexArrayObjectBindScope vao(vao_);
    vbo_.Bind();
    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 0);
    CHECK_GL(FATAL);
    glEnableVertexAttribArray(0);
    CHECK_GL(FATAL);

    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, normal));
    CHECK_GL(FATAL);
    glEnableVertexAttribArray(1);
    CHECK_GL(FATAL);

    // color
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, color)); 
    CHECK_GL(FATAL);
    glEnableVertexAttribArray(2);
    CHECK_GL(FATAL);

    // uv
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, uv));
    CHECK_GL(FATAL);
    glEnableVertexAttribArray(3);
    CHECK_GL(FATAL);
    glDrawArrays(GL_TRIANGLES, 0, vbo_.length());
    CHECK_GL(FATAL);
    vbo_.Unbind();
  }

  std::string Mesh::ToString() const {
    std::stringstream ss;
    ss << "mesh::Mesh(";
    ss << "vao=" << vao_ << ", ";
    ss << "vbo=" << vbo_;
    ss << ")";
    return ss.str();
  }

  void IndexedMesh::Render() {
    VertexArrayObjectBindScope vao(vao_);
    vbo_.Bind();
        // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 0);
    CHECK_GL(FATAL);
    glEnableVertexAttribArray(0);
    CHECK_GL(FATAL);

    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, normal));
    CHECK_GL(FATAL);
    glEnableVertexAttribArray(1);
    CHECK_GL(FATAL);

    // color
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, color)); 
    CHECK_GL(FATAL);
    glEnableVertexAttribArray(2);
    CHECK_GL(FATAL);

    // uv
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, uv));
    CHECK_GL(FATAL);
    glEnableVertexAttribArray(3);
    CHECK_GL(FATAL);
    ibo_.Bind();
    glDrawElements(GL_TRIANGLES, ibo_.length(), ibo_.type(), 0);
    CHECK_GL(FATAL);
    ibo_.Unbind();
    vbo_.Unbind();
  }

  std::string IndexedMesh::ToString() const {
    std::stringstream ss;
    ss << "mesh::IndexedMesh(";
    ss << "vao=" << vao_ << ", ";
    ss << "vbo=" << vbo_ << ", ";
    ss << "ibo=" << ibo_;
    ss << ")";
    return ss.str();
  }

  Mesh* NewMesh(const VertexArrayObject& vao, const VertexList& vertices) {
    VertexArrayObjectBindScope scope(vao);
    return new Mesh(vao, vertices);
  }

  Mesh* NewMesh(const VertexArrayObject& vao, const VertexList& vertices, const IndexList& indices) {
    VertexArrayObjectBindScope scope(vao);
    return new IndexedMesh(vao, vertices, indices);
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

    return NewMesh(BUILTIN_VAO(UVSphere), vertices, indices);
  }

  Mesh* NewPlaneXY(const uint64_t xSize, const uint64_t ySize) {
    if(xSize == 0 || ySize == 0)
      return nullptr;
    const uint64_t total_indices = xSize * ySize * 6;
    const uint64_t total_vertices = (xSize + 1) * (ySize + 1);
    const glm::vec3 offset = glm::vec3(-1.0f, -1.0f, 0.0f);
    const float xStep = 2.0f / xSize;
    const float yStep = 2.0f / ySize;
    const float uStep = 1.0f / xSize;
    const float vStep = 1.0f / ySize;

    VertexList vertices;
    for(auto x = 0; x <= xSize; x++) {
      for(auto y = 0; y <= ySize; y++) {
        const auto i = y + x * (ySize + 1);
        vertices.push_back(Vertex {
          .pos = glm::vec3(x * xStep, y * yStep, 0.0f) + offset,
          .uv = glm::vec2(x * uStep, (ySize - y) * vStep),
        });
      }
    }

    IndexList indices;
    for(auto v = 0; v < total_vertices - ySize - 2; v++) {
      if((v + 1) % (ySize + 1) == 0)
        v++;
      indices.push_back(v);
      indices.push_back(v + 1);
      indices.push_back(v + ySize + 1);
      indices.push_back(v + 1);
      indices.push_back(v + ySize + 2);
      indices.push_back(v + ySize + 1);
    }
    return NewMesh(BUILTIN_VAO(PlaneXY), vertices, indices);
  }

  Mesh* NewPlaneXZ(const uint64_t xSize, const uint64_t zSize) {
    if(xSize == 0 || zSize == 0)
      return nullptr;
    const uint64_t total_indices = xSize * zSize * 6;
    const uint64_t total_vertices = (xSize + 1) * (zSize + 1);
    const glm::vec3 offset = glm::vec3(-1.0f, -1.0f, 0.0f);
    const float xStep = 2.0f / xSize;
    const float zStep = 2.0f / zSize;
    const float uStep = 1.0f / xSize;
    const float vStep = 1.0f / zSize;

    VertexList vertices;
    for(auto x = 0; x <= xSize; x++) {
      for(auto z = 0; z <= zSize; z++) {
        const auto i = z + x * (zSize + 1);
        vertices.push_back(Vertex {
          .pos = glm::vec3(x * xStep, z * zStep, 0.0f) + offset,
          .uv = glm::vec2(x * uStep, (zSize - z) * vStep),
        });
      }
    }

    IndexList indices;
    for(auto v = 0; v < total_vertices - zSize - 2; v++) {
      if((v + 1) % (zSize + 1) == 0)
        v++;
      indices.push_back(v);
      indices.push_back(v + 1);
      indices.push_back(v + zSize + 1);
      indices.push_back(v + 1);
      indices.push_back(v + zSize + 2);
      indices.push_back(v + zSize + 1);
    }
    return NewMesh(BUILTIN_VAO(PlaneXZ), vertices, indices);
  }

  static const float Z = (1.0f + sqrt(5.0f)) / 2.0f;
  static constexpr const glm::vec2 UV = glm::vec2(1.0f / 11.0f, 1.0f / 3.0f);
  static constexpr const uint32_t kIcoVertexCount = 22;
  static constexpr const uint32_t kIcoIndexCount = 60;
  static const glm::vec3 kIcoVertices[] = {
    glm::vec3( 0, -1, -Z),
    glm::vec3(-1, -Z,  0), 
    glm::vec3( Z,  0, -1), 
    glm::vec3( 1, -Z,  0),
    glm::vec3( 1,  Z,  0),
    glm::vec3(-1, -Z,  0),
    glm::vec3( Z,  0,  1),
    glm::vec3( 0, -1,  Z),
    glm::vec3( 1,  Z,  0),
    glm::vec3(-1, -Z,  0), 
    glm::vec3( 0,  1,  Z),
    glm::vec3(-Z,  0,  1),
    glm::vec3( 1,  Z,  0), 
    glm::vec3(-1, -Z,  0), 
    glm::vec3(-1,  Z,  0),
    glm::vec3(-Z,  0, -1),
    glm::vec3( 1,  Z,  0),
    glm::vec3(-1, -Z,  0),
    glm::vec3( 0,  1, -Z),
    glm::vec3( 0, -1, -Z), 
    glm::vec3( 1,  Z,  0), 
    glm::vec3( Z,  0, -1) 
  };
  static const glm::vec2 kIcoUvs[] = {
    UV * glm::vec2( 0, 1), 
    UV * glm::vec2( 1, 0), 
    UV * glm::vec2( 1, 2),
    UV * glm::vec2( 2, 1),
    UV * glm::vec2( 2, 3),
    UV * glm::vec2( 3, 0),
    UV * glm::vec2( 3, 2),
    UV * glm::vec2( 4, 1),
    UV * glm::vec2( 4, 3),
    UV * glm::vec2( 5, 0),
    UV * glm::vec2( 5, 2),
    UV * glm::vec2( 6, 1),
    UV * glm::vec2( 6, 3),
    UV * glm::vec2( 7, 0),
    UV * glm::vec2( 7, 2),
    UV * glm::vec2( 8, 1),
    UV * glm::vec2( 8, 3),
    UV * glm::vec2( 9, 0),
    UV * glm::vec2( 9, 2),
    UV * glm::vec2(10, 1),
    UV * glm::vec2(10, 3),
    UV * glm::vec2(11, 2)
  };
  static constexpr const uint32_t kIcoIndices[] = {
    2,  6,  4,
    6, 10,  8,
    10, 14, 12,
    14, 18, 16,
    18, 21, 20,
    0,  3,  2,
    2,  3,  6,
    3,  7,  6,
    6,  7, 10,
    7, 11, 10,
    10, 11, 14,
    11, 15, 14,
    14, 15, 18,
    15, 19, 18,
    18, 19, 21,
    0,  1,  3,
    3,  5,  7,
    7,  9, 11,
    11, 13, 15,
    15, 17, 19
  };

  typedef std::unordered_map<uint64_t, uint64_t> IcoEdgeMap;

  Mesh* NewIcosphere(const uint64_t resolution) {
    const auto rn = (uint64_t)pow(4, resolution);
    const auto total_indices = kIcoIndexCount * rn;
    const auto total_vertices = kIcoVertexCount + kIcoIndexCount * (1 - rn) / (1 - 4);

    VertexList vertices;
    vertices.reserve(total_vertices);

    IndexList indices;
    indices.reserve(total_indices);

    for(auto i = 0; i < kIcoVertexCount; i++) {
      vertices.push_back(Vertex {
        .pos = kIcoVertices[i],
        .uv = kIcoUvs[i],
      });
    }

    for(auto i = 0; i < kIcoIndexCount; i++) {
      indices.push_back(kIcoIndices[i]);
    }

    auto currentIndex = kIcoIndexCount;
    auto currentVertex = kIcoVertexCount;
    for(auto r = 0; r < resolution; r++) {
      IcoEdgeMap edge_map;
      auto index_count = currentIndex;
      for(auto t = 0; t < index_count; t += 3) {
        uint64_t midpoints[3] = {};
        for(auto e = 0; e < 3; e++) {
          auto first = indices[t + 3];
          auto second = indices[t + (t + e + 1) % 3];
          if(first > second)
            std::swap(first, second);
          uint64_t hash = first | second << 8;
          auto [ triangle, wasNewEdge ] = edge_map.insert({ hash, currentVertex });
          if(wasNewEdge) {
            vertices[currentVertex].pos = (vertices[first].pos + vertices[second].pos) / 2.0f;
            vertices[currentVertex].uv = (vertices[first].uv + vertices[second].uv) / 2.0f;
            currentVertex += 1;
          }
          midpoints[e] = triangle->second;
        }

        auto mid0 = midpoints[0];
        auto mid1 = midpoints[1];
        auto mid2 = midpoints[2];

        indices[currentIndex++] = indices[t];
        indices[currentIndex++] = mid0;
        indices[currentIndex++] = mid2;

        indices[currentIndex++] = indices[t + 1];
        indices[currentIndex++] = mid1;
        indices[currentIndex++] = mid0;

        indices[currentIndex++] = indices[t + 2];
        indices[currentIndex++] = mid2;
        indices[currentIndex++] = mid1;

        indices[t] = mid0;
        indices[t + 1] = mid1;
        indices[t + 2] = mid2;
      }
    }

    for(auto& vertex : vertices)
      vertex.pos = glm::normalize(vertex.pos);
    return NewMesh(BUILTIN_VAO(Icosphere), vertices, indices);
  }

  static const VertexList kCubeVertices = {
    // back
    { .pos = glm::vec3(-1.0f, -1.0f, -1.0f), .uv = glm::vec2(0.0f, 0.0f), .normal = glm::vec3(0.0f, 0.0f, -1.0f) },
    { .pos = glm::vec3( 1.0f, -1.0f, -1.0f), .uv = glm::vec2(1.0f, 0.0f), .normal = glm::vec3(0.0f, 0.0f, -1.0f)},
    { .pos = glm::vec3(1.0f,  1.0f, -1.0f), .uv = glm::vec2(1.0f, 1.0f), .normal = glm::vec3(0.0f, 0.0f, -1.0f) },
    { .pos = glm::vec3(1.0f,  1.0f, -1.0f), .uv = glm::vec2(1.0f, 1.0f), .normal = glm::vec3(0.0f, 0.0f, -1.0f) },
    { .pos = glm::vec3(-1.0f,  1.0f, -1.0f), .uv = glm::vec2(0.0f, 1.0f), .normal = glm::vec3(0.0f, 0.0f, -1.0f) },
    { .pos = glm::vec3(-1.0f, -1.0f, -1.0f), .uv = glm::vec2(0.0f, 0.0f), .normal = glm::vec3(0.0f, 0.0f, -1.0f) },

    // front
    { .pos = glm::vec3(-1.0f, -1.0f,  1.0f), .uv = glm::vec2(0.0f, 0.0f), .normal = glm::vec3(0.0f, 0.0f, 1.0f), },
    { .pos = glm::vec3(1.0f, -1.0f,  1.0f), .uv = glm::vec2(1.0f, 0.0f), .normal = glm::vec3(0.0f, 0.0f, 1.0f), },
    { .pos = glm::vec3(1.0f,  1.0f,  1.0f), .uv = glm::vec2(1.0f, 1.0f), .normal = glm::vec3(0.0f, 0.0f, 1.0f), },
    { .pos = glm::vec3(1.0f,  1.0f,  1.0f), .uv = glm::vec2(1.0f, 1.0f), .normal = glm::vec3(0.0f, 0.0f, 1.0f), },
    { .pos = glm::vec3(-1.0f,  1.0f,  1.0f), .uv = glm::vec2(0.0f, 1.0f), .normal = glm::vec3(0.0f, 0.0f, 1.0f), },
    { .pos = glm::vec3(-1.0f, -1.0f,  1.0f), .uv = glm::vec2(0.0f, 0.0f), .normal = glm::vec3(0.0f, 0.0f, 1.0f), },

    // left
    { .pos = glm::vec3(-1.0f,  1.0f,  1.0f), .uv = glm::vec2(1.0f, 0.0f), .normal = glm::vec3(-1.0f, 0.0f, 0.0f) },
    { .pos = glm::vec3(-1.0f,  1.0f, -1.0f), .uv = glm::vec2(1.0f, 1.0f), .normal = glm::vec3(-1.0f, 0.0f, 0.0f) },
    { .pos = glm::vec3(-1.0f, -1.0f, -1.0f), .uv = glm::vec2(0.0f, 1.0f), .normal = glm::vec3(-1.0f, 0.0f, 0.0f) },
    { .pos = glm::vec3(-1.0f, -1.0f, -1.0f), .uv = glm::vec2(0.0f, 1.0f), .normal = glm::vec3(-1.0f, 0.0f, 0.0f) },
    { .pos = glm::vec3(-1.0f, -1.0f,  1.0f), .uv = glm::vec2(0.0f, 0.0f), .normal = glm::vec3(-1.0f, 0.0f, 0.0f) },
    { .pos = glm::vec3(-1.0f,  1.0f,  1.0f), .uv = glm::vec2(1.0f, 0.0f), .normal = glm::vec3(-1.0f, 0.0f, 0.0f) },

    // right
    { .pos = glm::vec3(1.0f,  1.0f,  1.0f), .uv = glm::vec2(1.0f, 0.0f), .normal = glm::vec3(1.0f, 0.0f, 0.0f) },
    { .pos = glm::vec3(1.0f,  1.0f, -1.0f), .uv = glm::vec2(1.0f, 1.0f), .normal = glm::vec3(1.0f, 0.0f, 0.0f) },
    { .pos = glm::vec3(1.0f, -1.0f, -1.0f), .uv = glm::vec2(0.0f, 1.0f), .normal = glm::vec3(1.0f, 0.0f, 0.0f) },
    { .pos = glm::vec3(1.0f, -1.0f, -1.0f), .uv = glm::vec2(0.0f, 1.0f), .normal = glm::vec3(1.0f, 0.0f, 0.0f) },
    { .pos = glm::vec3(1.0f, -1.0f,  1.0f), .uv = glm::vec2(0.0f, 0.0f), .normal = glm::vec3(1.0f, 0.0f, 0.0f) },
    { .pos = glm::vec3(1.0f,  1.0f,  1.0f), .uv = glm::vec2(1.0f, 0.0f), .normal = glm::vec3(1.0f, 0.0f, 0.0f) },

    // bottom
    { .pos = glm::vec3(-1.0f, -1.0f, -1.0f), .uv = glm::vec2(0.0f, 1.0f), .normal = glm::vec3(0.0f, -1.0f, 0.0f) },
    { .pos = glm::vec3(1.0f, -1.0f, -1.0f), .uv = glm::vec2(1.0f, 1.0f), .normal = glm::vec3(0.0f, -1.0f, 0.0f) },
    { .pos = glm::vec3(1.0f, -1.0f,  1.0f), .uv = glm::vec2(1.0f, 0.0f), .normal = glm::vec3(0.0f, -1.0f, 0.0f) },
    { .pos = glm::vec3(1.0f, -1.0f,  1.0f), .uv = glm::vec2(1.0f, 0.0f), .normal = glm::vec3(0.0f, -1.0f, 0.0f) },
    { .pos = glm::vec3(-1.0f, -1.0f,  1.0f), .uv = glm::vec2(0.0f, 0.0f), .normal = glm::vec3(0.0f, -1.0f, 0.0f) },
    { .pos = glm::vec3(-1.0f, -1.0f, -1.0f), .uv = glm::vec2(0.0f, 1.0f), .normal = glm::vec3(0.0f, -1.0f, 0.0f) },

    // top 
    { .pos = glm::vec3(-1.0f,  1.0f, -1.0f), .uv = glm::vec2(0.0f, 1.0f), .normal = glm::vec3(0.0f, 1.0f, 0.0f) },
    { .pos = glm::vec3(1.0f,  1.0f, -1.0f), .uv = glm::vec2(1.0f, 1.0f), .normal = glm::vec3(0.0f, 1.0f, 0.0f) },
    { .pos = glm::vec3(1.0f,  1.0f,  1.0f), .uv = glm::vec2(1.0f, 0.0f), .normal = glm::vec3(0.0f, 1.0f, 0.0f) },
    { .pos = glm::vec3(1.0f,  1.0f,  1.0f), .uv = glm::vec2(1.0f, 0.0f), .normal = glm::vec3(0.0f, 1.0f, 0.0f) },
    { .pos = glm::vec3(-1.0f,  1.0f,  1.0f), .uv = glm::vec2(0.0f, 0.0f), .normal = glm::vec3(0.0f, 1.0f, 0.0f) },
    { .pos = glm::vec3(-1.0f,  1.0f, -1.0f), .uv = glm::vec2(0.0f, 1.0f), .normal = glm::vec3(0.0f, 1.0f, 0.0f) },
  };

  Mesh* NewCube() {
    return NewMesh(BUILTIN_VAO(Cube), kCubeVertices);
  }
}