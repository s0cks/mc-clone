#ifndef MCC_SKYBOX_H
#define MCC_SKYBOX_H

#include <vector>
#include "mcc/gfx.h"
#include "mcc/texture/texture.h"

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
    public:
      explicit VertexBuffer(const BufferObjectId id = kInvalidBufferObject):
        VertexBufferTemplate(id) {  
      }
      explicit VertexBuffer(const Vertex* vertices, const uint64_t num_vertices):
        VertexBufferTemplate(vertices, num_vertices) {
        glEnableVertexAttribArray(0);
        CHECK_GL(FATAL);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 0);
        CHECK_GL(FATAL);

        // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, uv));
        // CHECK_GL(FATAL);
        // glEnableVertexAttribArray(1);
        // CHECK_GL(FATAL);
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

    class Skybox {
      DEFINE_NON_COPYABLE_TYPE(Skybox);
    private:
      TextureRef texture_;
      VertexBuffer vbo_;

      Skybox(const std::string& cube_map_filename,
             const VertexList& vertices):
        vbo_(vertices),
        texture_(GetTexture("graycloud")) {
      }

      static void OnPostInit();
    public:
      Skybox() = delete;
      ~Skybox() = default;

      void Render();
    public:
      static void Init();
      static Skybox* Get();
      static Skybox* New(const std::string& cube_map_filename);
    };
  }
}

#endif //MCC_SKYBOX_H