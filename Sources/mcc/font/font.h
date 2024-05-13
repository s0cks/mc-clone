#ifndef MCC_FONT_H
#define MCC_FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <cstdio>
#include <cstdlib>
#include <glog/logging.h>

#include "mcc/font/glyph.h"
#include "mcc/font/font_mesh.h"
#include "mcc/program/program.h"

namespace mcc::font {
  typedef FT_Face FontFace;

  class Font {
    friend class FontRenderer;

    static constexpr const FontSize kDefaultFontSize = 32;
  protected:
    Program* shader_;
    GlyphMap chars_;
    FontMesh mesh_;
  public:
    Font(const std::string& filename, const FontSize size = kDefaultFontSize);
    virtual ~Font() = default;
  };

  void Initialize();
  void LoadTrueTypeFont(const std::string& name, FontFace* result);
  void SetFont(FontFace* font);
  FontFace* GetFont();
  FontFace* GetArialFont();

  bool LoadTrueTypeFontFrom(const std::string& filename, Font* result);
}

#endif //MCC_FONT_H