#ifndef MCC_GUI_H
#define MCC_GUI_H

#include <memory>
#include "mcc/gfx.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear.h"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

namespace nk {
  typedef struct nk_context Context;
  typedef std::shared_ptr<Context> ContextPtr;

  static inline ContextPtr
  NewContext() {
    auto ctx = std::make_shared<Context>();
    nk_init_default(ctx.get(), 0);
    return ctx;
  }

  typedef struct nk_colorf Color;

  typedef struct nk_buffer Buffer;
  typedef struct nk_draw_null_texture DrawNullTexture;
  typedef struct nk_font_atlas FontAtlas;
  typedef struct nk_draw_command DrawCommand;
}

namespace mcc::gui {
  struct Vertex {
    glm::vec2 position;
    glm::vec2 uv;
    glm::u8vec4 color;

    friend std::ostream& operator<<(std::ostream& stream, const Vertex& rhs) {
      stream << "gui::Vertex(";
      stream << "pos=" << glm::to_string(rhs.position) << ", ";
      stream << "uv=" << glm::to_string(rhs.uv) << ", ";
      stream << "color=" << glm::to_string(rhs.color);
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

  class VertexBuffer : public VertexBufferTemplate<Vertex, kStreamUsage> {
  public:
    explicit VertexBuffer(const BufferObjectId id):
      VertexBufferTemplate(id) {
    }
    VertexBuffer():
      VertexBufferTemplate() {
    }
    VertexBuffer(const Vertex* vertices, const uint64_t num_vertices):
      VertexBufferTemplate(vertices, num_vertices) {
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 0);
      CHECK_GL(FATAL);
      glEnableVertexAttribArray(0);
      CHECK_GL(FATAL);

      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, uv));
      CHECK_GL(FATAL);
      glEnableVertexAttribArray(1);
      CHECK_GL(FATAL);

      glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, color));
      CHECK_GL(FATAL);
      glEnableVertexAttribArray(2);
      CHECK_GL(FATAL);
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

    friend std::ostream& operator<<(std::ostream& stream, const VertexBuffer& rhs) {
      stream << "gui::VertexBuffer(";
      stream << "id=" << rhs.id() << ", ";
      stream << "vertex_size=" << rhs.vertex_size() << ", ";
      stream << "length=" << rhs.length() << ", ";
      stream << "size=" << rhs.size();
      stream << "usage=" << rhs.usage() << ", ";
      stream << "target=" << rhs.target();
      stream << ")";
      return stream;
    }
  };

  typedef GLushort Index;
  typedef std::vector<Index> IndexList;

  static inline std::ostream&
  operator<<(std::ostream& stream, const IndexList& rhs) {
    stream << "(";
    for(auto idx = 0; idx < rhs.size(); idx++) {
      stream << rhs[idx];
      if((idx + 1) < rhs.size())
        stream << ", ";
    }
    stream << ")";
    return stream;
  }

  class IndexBuffer : public IndexBufferTemplate<Index, GL_UNSIGNED_SHORT, kStreamUsage> {
  public:
    explicit IndexBuffer(const BufferObjectId id):
      IndexBufferTemplate(id) {
    }
    IndexBuffer(const Index* indices, const uint64_t num_indices):
      IndexBufferTemplate(indices, num_indices) {
    }
    IndexBuffer() = default;
    explicit IndexBuffer(const IndexList& indices):
      IndexBuffer(&indices[0], indices.size()) {
    }
    IndexBuffer(const IndexBuffer& rhs):
      IndexBufferTemplate(rhs) {
    }
    ~IndexBuffer() override = default;

    void operator=(const IndexBuffer& rhs) {
      IndexBufferTemplate::operator=(rhs);
    }

    void operator=(const BufferObjectId& rhs) {
      BufferObject::operator=(rhs);
    }

    friend std::ostream& operator<<(std::ostream& stream, const IndexBuffer& rhs) {
      stream << "gui::IndexBuffer(";
      stream << "id=" << rhs.id() << ", ";
      stream << "index_size=" << rhs.index_size() << ", ";
      stream << "length=" << rhs.length() << ", ";
      stream << "size=" << rhs.size();
      stream << "usage=" << rhs.usage() << ", ";
      stream << "target=" << rhs.target();
      stream << ")";
      return stream;
    }
  };

  class Screen {
  public:
    enum Position {

    };
  public:
    Screen() = default;
    ~Screen() = default;

    virtual void Render();
  private:
    static void OnPostInit();
  public:
    static void Init();
    static void NewFrame();
    static void TestScreen();
    static void RenderScreen(const glm::mat4 projection, enum nk_anti_aliasing AA = NK_ANTI_ALIASING_OFF, int max_vertex_buffer = MAX_VERTEX_BUFFER, int max_element_buffer = MAX_ELEMENT_BUFFER);
  };
}

#endif //MCC_GUI_H