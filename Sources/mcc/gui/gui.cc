#include "mcc/gui/gui.h"
#include "mcc/shader/shader.h"
#include "mcc/thread_local.h"
#include "mcc/window.h"
#include "mcc/engine/engine.h"

#include "mcc/flags.h"
#include "mcc/terrain/terrain.h"
#include "mcc/renderer/renderer.h"

namespace mcc::gui {
  static nk::Buffer cmds_;
  static nk::Context ctx_;
  static nk::FontAtlas fonts_;
  static nk::DrawNullTexture draw_null;

  static Shader shader_;
  static VertexArrayObject vao_(kInvalidVertexArrayObject);
  static VertexBuffer vbo_(kInvalidBufferObject);
  static IndexBuffer ibo_(kInvalidBufferObject);
  static int width_;
  static int height_;
  static int display_width_;
  static int display_height_;
  static glm::vec2 fb_scale_;
  static glm::vec2 scroll_;
  struct nk_colorf bg_ {
    .r = 1.0f,
    .g = 0.18f, 
    .b = 0.24f, 
    .a = 1.0f,
  };

  static GLuint font_tex_;

  static inline void
  FontStashBegin(nk::FontAtlas** atlas) {
    nk_font_atlas_init_default(&fonts_);
    nk_font_atlas_begin(&fonts_);
    (*atlas) = &fonts_;
  }

  static inline void
  UploadAtlas(const void* img, const int width, const int height) {
    glGenTextures(1, &font_tex_);
    glBindTexture(GL_TEXTURE_2D, font_tex_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
  }

  static inline void
  FontStashEnd() {
    int width, height;
    const void* img = nk_font_atlas_bake(&fonts_, &width, &height, NK_FONT_ATLAS_RGBA32);
    UploadAtlas(img, width, height);
    nk_font_atlas_end(&fonts_, nk_handle_id((int) font_tex_), &draw_null);
    if(fonts_.default_font)
      nk_style_set_font(&ctx_, &fonts_.default_font->handle);
  }

  void Screen::Init() {
    Engine::OnPostInit(&OnPostInit);
  }

  static inline void
  CreateShader() {
    shader_ = CompileShader("gui");
  }

  static inline void
  CreateBufferObjects() {
    vao_ = VertexArrayObject();
    VertexArrayObjectBindScope vao(vao_);
    vbo_ = VertexBuffer();
    const auto attr_pos = glGetAttribLocation(shader_.GetShaderId(), "pos");
    glVertexAttribPointer(attr_pos, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 0);
    CHECK_GL(FATAL);
    glEnableVertexAttribArray(attr_pos);
    CHECK_GL(FATAL);

    const auto attr_uv = glGetAttribLocation(shader_.GetShaderId(), "uv");
    glVertexAttribPointer(attr_uv, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, uv));
    CHECK_GL(FATAL);
    glEnableVertexAttribArray(attr_uv);
    CHECK_GL(FATAL);

    const auto attr_color = glGetAttribLocation(shader_.GetShaderId(), "color");
    glVertexAttribPointer(attr_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, color));
    CHECK_GL(FATAL);
    glEnableVertexAttribArray(attr_color);
    CHECK_GL(FATAL);
    ibo_ = IndexBuffer();

    glBindTexture(GL_TEXTURE_2D, 0);
    vbo_.Unbind();
    ibo_.Unbind();
    vao_.Unbind();
  }

  nk::Context* Screen::GetNuklearContext() {
    return &ctx_;
  }

  void Screen::OnPostInit() {
    nk_init_default(&ctx_, NULL);
    nk_buffer_init_default(&cmds_);
    CreateShader();
    CreateBufferObjects();
    {
      nk::FontAtlas* atlas;
      FontStashBegin(&atlas);
      FontStashEnd();
    }
  }

  void Screen::NewFrame() {
    DLOG(INFO) << "new frame....";
    const auto window = Window::GetHandle();
    glfwGetWindowSize(window, &width_, &height_);
    CHECK_GL(FATAL);
    glfwGetFramebufferSize(window, &display_width_, &display_height_);
    CHECK_GL(FATAL);
    fb_scale_[0] = static_cast<float>(display_width_) / static_cast<float>(width_);
    fb_scale_[1] = static_cast<float>(display_height_) / static_cast<float>(height_);

    nk_input_begin(&ctx_);
    {
      double x, y;
      glfwGetCursorPos(window, &x, &y);
      nk_input_motion(&ctx_, (int) x, (int) y);
      nk_input_button(&ctx_, NK_BUTTON_LEFT, (int) x, (int) y, glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
      nk_input_button(&ctx_, NK_BUTTON_MIDDLE, (int) x, (int) y, glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
      nk_input_button(&ctx_, NK_BUTTON_RIGHT, (int) x, (int) y, glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
    }
    nk_input_end(&ctx_);
  }

  void Screen::TestScreen() {
    if (nk_begin(&ctx_, "Settings", nk_rect(0, 0, width_ / 4, height_), NK_WINDOW_TITLE)) {

    }
    nk_end(&ctx_);
  }

  void Screen::RenderScreen(const glm::mat4 projection, enum nk_anti_aliasing AA, int max_vertex_buffer, int max_element_buffer) {
    nk::Buffer vbuf, ebuf;
    glEnable(GL_BLEND);
    CHECK_GL(FATAL);
    glBlendEquation(GL_FUNC_ADD);
    CHECK_GL(FATAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    CHECK_GL(FATAL);
    glDisable(GL_CULL_FACE);
    CHECK_GL(FATAL);
    glDisable(GL_DEPTH_TEST);
    CHECK_GL(FATAL);
    glEnable(GL_SCISSOR_TEST);
    CHECK_GL(FATAL);
    glActiveTexture(GL_TEXTURE0);
    CHECK_GL(FATAL);

    shader_.ApplyShader();
    shader_.SetInt("tex", 0);
    shader_.SetMat4("projection", projection);
    shader_.ApplyShader();
    glViewport(0,0, (GLsizei) display_width_, (GLsizei) display_height_);
    CHECK_GL(FATAL);
    {
      const nk::DrawCommand* cmd;

      const nk_draw_index* offset = NULL;

      vao_.Bind();
      vbo_.Bind();
      ibo_.Bind();
      
      vbo_.BufferData(max_vertex_buffer);
      ibo_.BufferData(max_element_buffer);

      void* vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
      CHECK_GL(FATAL);
      void* elements = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
      CHECK_GL(FATAL);
      {
          /* fill convert configuration */
        struct nk_convert_config config;
        static const struct nk_draw_vertex_layout_element vertex_layout[] = {
            {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, offsetof(Vertex, position)},
            {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, offsetof(Vertex, uv)},
            {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, offsetof(Vertex, color)},
            {NK_VERTEX_LAYOUT_END}
        };
        memset(&config, 0, sizeof(config));
        config.vertex_layout = vertex_layout;
        config.vertex_size = sizeof(Vertex);
        config.vertex_alignment = NK_ALIGNOF(Vertex);
        config.tex_null = draw_null;
        config.circle_segment_count = 22;
        config.curve_segment_count = 22;
        config.arc_segment_count = 22;
        config.global_alpha = 1.0f;
        config.shape_AA = AA;
        config.line_AA = AA;

        /* setup buffers to load vertices and elements */
        nk_buffer_init_fixed(&vbuf, vertices, (size_t)max_vertex_buffer);
        nk_buffer_init_fixed(&ebuf, elements, (size_t)max_element_buffer);
        nk_convert(&ctx_, &cmds_, &vbuf, &ebuf, &config);
      }
      glUnmapBuffer(GL_ARRAY_BUFFER);
      CHECK_GL(FATAL);
      glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
      CHECK_GL(FATAL);

      /* iterate over and execute each draw command */
      nk_draw_foreach(cmd, &ctx_, &cmds_)
      {
        if (!cmd->elem_count) continue;
        glBindTexture(GL_TEXTURE_2D, (GLuint)cmd->texture.id);
        CHECK_GL(FATAL);
        glScissor(
            (GLint)(cmd->clip_rect.x * fb_scale_[0]),
            (GLint)((height_ - (GLint)(cmd->clip_rect.y + cmd->clip_rect.h)) * fb_scale_[1]),
            (GLint)(cmd->clip_rect.w * fb_scale_[0]),
            (GLint)(cmd->clip_rect.h * fb_scale_[1]));
        CHECK_GL(FATAL);
        glDrawElements(GL_TRIANGLES, (GLsizei)cmd->elem_count, GL_UNSIGNED_SHORT, offset);
        CHECK_GL(FATAL);
        offset += cmd->elem_count;
      }
      nk_clear(&ctx_);
    }

    glUseProgram(0);
    CHECK_GL(FATAL);
    vbo_.Unbind();
    ibo_.Unbind();
    vao_.Unbind();
    glDisable(GL_BLEND);
    CHECK_GL(FATAL);
    glDisable(GL_SCISSOR_TEST);
    CHECK_GL(FATAL);
  }
}