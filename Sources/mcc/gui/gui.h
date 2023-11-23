#ifndef MCC_GUI_H
#define MCC_GUI_H

#include <memory>
#include "mcc/gfx.h"
#include "mcc/index_buffer.h"

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