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

#include "mcc/pipeline.h"

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
    static void RenderScreen(const glm::mat4 projection, enum nk_anti_aliasing AA = NK_ANTI_ALIASING_ON, int max_vertex_buffer = MAX_VERTEX_BUFFER, int max_element_buffer = MAX_ELEMENT_BUFFER);

    static nk::Context* GetNuklearContext();
  };

  class RenderScreenPipeline : public Pipeline {
  protected:
    glm::mat4 projection_;
  public:
    explicit RenderScreenPipeline(const glm::mat4& projection):
      Pipeline(),
      projection_(projection) {
    }
    explicit RenderScreenPipeline(const Dimension& size):
      RenderScreenPipeline(glm::ortho(0.0f, size[0] * 1.0f, size[1] * 1.0f, 0.0f)) {
    }
    ~RenderScreenPipeline() override = default;

    void Render() override {
      VLOG(3) << "drawing screen....";
      gui::Screen::RenderScreen(projection_);
    }
  };
}

#endif //MCC_GUI_H