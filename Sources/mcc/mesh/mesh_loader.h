#ifndef MCC_MESH_LOADER_H
#define MCC_MESH_LOADER_H

#include <uv.h>
#include "mcc/mesh/mesh.h"
#include "mcc/mesh/obj_parser.h"

namespace mcc::mesh {
  class MeshLoader {
  protected:
    MeshLoader() = default;
  public:
    virtual ~MeshLoader() = default;
    virtual Mesh* LoadMesh() = 0;
  };

  class ObjMeshLoader : public MeshLoader {
  private:
    std::string filename_;
    Vao* vao_;
    VertexList mesh_verts_;
    std::vector<glm::vec3> vertices_;
    std::vector<glm::vec3> normals_;
    std::vector<glm::vec2> uvs_;
    ObjParser::Config config_;

    static inline bool
    OnVertexParsed(const ObjParser* parser, uint64_t idx, const glm::vec3& vertex) {
      auto loader = (ObjMeshLoader*)parser->data();
      loader->vertices_.push_back(vertex);
      return true;
    }

    static inline bool
    OnNormalParsed(const ObjParser* parser, const uint64_t idx, const glm::vec3& normal) {
      auto loader = (ObjMeshLoader*)parser->data();
      loader->normals_.push_back(normal);
      return true;
    }

    static inline bool
    OnUvParsed(const ObjParser* parser, const uint64_t idx, const glm::vec2& uv) {
      auto loader = (ObjMeshLoader*)parser->data();
      loader->uvs_.push_back(uv);
      return true;
    }

    static inline bool
    OnFaceParsed(const ObjParser* parser, const uint64_t face_idx, FaceVertex* vertices, const uint64_t num_vertices) {
      auto loader = (ObjMeshLoader*)parser->data();
      for(auto idx = 0; idx < num_vertices; idx++) {
        const auto& vert = vertices[idx];
        const auto& vertex = loader->vertices_[vert[0]];
        const auto& normal = vert[2] > 0 ? loader->normals_[vert[2]] : glm::vec3(0.0f);
        const auto& uv = vert[1] > 0 ? loader->uvs_[vert[1]] : glm::vec2(0.0f);
        const auto face_vert = Vertex {
          .pos = vertex,
          .normal = normal,
          .uv = uv,
          .color = glm::vec3(1.0f),
        };
        loader->mesh_verts_.push_back(face_vert);
      }
      return true;
    }
  public:
    explicit ObjMeshLoader(Vao* vao, const std::string& filename):
      MeshLoader(),
      vao_(vao),
      filename_(filename),
      vertices_(),
      normals_(),
      uvs_(),
      config_(),
      mesh_verts_() {
      config_.on_vertex_parsed = &OnVertexParsed;
      config_.on_normal_parsed = &OnNormalParsed;
      config_.on_uv_parsed = &OnUvParsed;
      config_.on_face_parsed = &OnFaceParsed;
    }
    ~ObjMeshLoader() override = default;
    
    Mesh* LoadMesh() override { //TODO: convert to std::optional
      auto file = fopen(filename_.c_str(), "rb");
      if(!file) {
        LOG(ERROR) << "failed to open " << filename_ << ".";
        return nullptr;
      }

      const auto start = uv_hrtime();
      ObjParser parser(config_, file, this);
      if(!parser.ParseFile()) {
        LOG(ERROR) << "failed to parse " << filename_ << ".";
        fclose(file); //TODO: prolly should check fclose here eventually
        return nullptr;
      }
      const auto end = uv_hrtime();

      if(mesh_verts_.empty()) {
        LOG(ERROR) << "failed to parse mesh vertices from " << filename_ << ".";
        fclose(file); //TODO: prolly should check fclose here eventually
        return nullptr;
      }

#ifdef MCC_DEBUG
      DLOG(INFO) << "parsed mesh from " << filename_ << " in " << ((end - start) / NSEC_PER_MSEC) << "ms";
      DLOG(INFO) << " - " << vertices_.size() << " vertices.";
      DLOG(INFO) << " - " << normals_.size() << " normals.";
      DLOG(INFO) << " - " << uvs_.size() << " uvs.";
      DLOG(INFO) << " - " << mesh_verts_.size() << " faces.";
#endif//MCC_DEBUG
      fclose(file); //TODO: prolly should check fclose here eventually
      return NewMesh(vao_, mesh_verts_);
    }
  };
}

#endif //MCC_MESH_LOADER_H