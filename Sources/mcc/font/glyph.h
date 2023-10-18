#ifndef MCC_GLYPH_H
#define MCC_GLYPH_H

#include <map>
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
  typedef std::map<GLchar, Glyph> GlyphMap;

  class GlyphMapGenerator {
  protected:
    FT_Face& face_;
    FT_UInt height_;

    GlyphMapGenerator(FT_Face& face, FT_UInt height):
      face_(face),
      height_(height) {
    }

    void GenerateCharacterMap(GlyphMap& chars) {
      FT_Set_Pixel_Sizes(face_, 0, height_);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      for(unsigned char c = 0; c < 128; c++) {
        if(FT_Load_Char(face_, c, FT_LOAD_RENDER)) {
          LOG(ERROR) << "failed to load glyph for '" << c << "'";
          continue;
        }

        auto& glyph = face_->glyph;
        chars.insert(std::pair<char, Glyph>(c, Glyph(glyph)));
      }
      glBindTexture(GL_TEXTURE_2D, 0);
    }
  public:
    virtual ~GlyphMapGenerator() = default;
    DEFINE_NON_COPYABLE_TYPE(GlyphMapGenerator);
  public:
    static void Generate(FT_Face& face, GlyphMap& chars, const FT_UInt height = 48);
    static void Generate(const std::string& name, GlyphMap& chars, const FT_UInt height = 48);
  };
}

#endif //MCC_GLYPH_H