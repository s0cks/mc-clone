#ifndef MCC_GLYPH_H
#define MCC_GLYPH_H

#include <vector>

#include "mcc/common.h"
#include "mcc/font/truetype.h"
#include "mcc/font/font_mesh.h"

namespace mcc::font {
  typedef GLuint GlyphTextureId;

  static inline void
  GenerateGlyphTexture(GlyphTextureId& texture, FT_Bitmap& bitmap) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmap.width, bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  struct Glyph {
    friend class FontRenderer;
  private:
    void Draw(const glm::vec2 pos, const float scale, FontMesh& mesh);
  public:
    GlyphTextureId texture;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance;

    Glyph() = default;
    Glyph(FT_Bitmap& bitmap, FT_Int left, FT_Int top, FT_Vector& advance):
      texture(),
      size(glm::ivec2(bitmap.width, bitmap.rows)),
      bearing(glm::ivec2(left, top)),
      advance(static_cast<GLuint>(advance.x)) {
      GenerateGlyphTexture(texture, bitmap);
    }
    Glyph(FT_GlyphSlot glyph):
      Glyph(glyph->bitmap, glyph->bitmap_left, glyph->bitmap_top, glyph->advance) {
    }
    Glyph(const Glyph& rhs) = default;
    ~Glyph() = default;
  };

  typedef std::vector<Glyph> GlyphList;
}

#endif //MCC_GLYPH_H