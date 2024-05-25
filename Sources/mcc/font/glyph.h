#ifndef MCC_GLYPH_H
#define MCC_GLYPH_H

#include <vector>

#include "mcc/common.h"
#include "mcc/font/truetype.h"
#include "mcc/font/font_mesh.h"

#include "mcc/texture/texture_id.h"

namespace mcc::font {
  struct Glyph {
    friend class FontRenderer;
    DEFINE_DEFAULT_COPYABLE_TYPE(Glyph);
  private:
    void Draw(const glm::vec2 pos, FontMesh& mesh);
  public:
    TextureId texture;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance;

    Glyph() = default;
    Glyph(FT_Bitmap& bitmap, FT_Int left, FT_Int top, FT_Vector& advance);
    explicit Glyph(FT_GlyphSlot glyph):
      Glyph(glyph->bitmap, glyph->bitmap_left, glyph->bitmap_top, glyph->advance) {
    }
    ~Glyph() = default;

    friend std::ostream& operator<<(std::ostream& stream, const Glyph& rhs) {
      stream << "font::Glyph(";
      stream << "texture=" << rhs.texture << ", ";
      stream << "size=" << glm::to_string(rhs.size) << ", ";
      stream << "bearing=" << glm::to_string(rhs.bearing) << ", ";
      stream << "advance=" << rhs.advance;
      stream << ")";
      return stream;
    }
  };

  typedef std::vector<Glyph> GlyphList;
}

#endif //MCC_GLYPH_H