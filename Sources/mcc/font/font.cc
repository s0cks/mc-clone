#include "mcc/font/font.h"
#include "mcc/flags.h"
#include "mcc/thread_local.h"

#include "mcc/camera/camera.h"
#include "mcc/shader/shader.h"
#include "mcc/program/program_scope.h"

#include "mcc/window/window.h"

namespace mcc::font {
  static FT_Library ft;
  static FontFace arial;
  static ThreadLocal<FontFace> font;

  void Initialize() {
    if(FT_Init_FreeType(&ft))
      LOG(FATAL) << "failed to initialize FreeType library.";
  }

  void SetFont(FontFace* value) {
    font.Set(value);
  }

  FontFace* GetFont() {
    return font.Get();
  }

  FontFace* GetArialFont() {
    return &arial;
  }

  static inline std::string
  GetTrueTypeFilename(const std::string& name) {
    return name + ".ttf";
  }

  static inline std::string
  GetTrueTypeFile(const std::string& name) {
    return FLAGS_resources + "/fonts/" + GetTrueTypeFilename(name);
  }

  void LoadTrueTypeFont(const std::string& name, FontFace* result) {
    const auto filename = GetTrueTypeFile(name);
    DLOG(INFO) << "loading TrueType font from:" << filename;
    const auto error = FT_New_Face(ft, filename.c_str(), 0, result);
    LOG_IF(FATAL, error != 0) << "failed to load TrueType font from " << filename << ": " << error;
  }

  Font::Font(const std::string& name, const FontSize size):
    shader_(Program::FromJson(fmt::format("program:text", name))),
    chars_(),
    mesh_() {
    const auto window = Window::Get();
    MCC_ASSERT(window);
    const auto sz = window->GetSize();
    const auto projection = glm::ortho(0.0f, sz[0] * 1.0f, 0.0f, sz[1] * 1.0f, 0.1f, 1000.0f);
    program::ApplyProgramScope prog(shader_);
    prog.Set("projection", projection);
    GlyphMapGenerator::Generate(name, chars_, size);
  }

  std::string Font::ToString() const {
    std::stringstream ss;
    ss << "Font(";
    ss << ")";
    return ss.str();
  }
}