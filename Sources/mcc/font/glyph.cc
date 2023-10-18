#include "mcc/font/glyph.h"
#include "mcc/font/font.h"

namespace mcc::font {
  void GlyphMapGenerator::Generate(FT_Face& face, GlyphMap& chars, const FT_UInt height) {
    GlyphMapGenerator gen(face, height);
    return gen.GenerateCharacterMap(chars);
  }

  void GlyphMapGenerator::Generate(const std::string& name, GlyphMap& chars, const FT_UInt height) {
    FontFace face;
    LoadTrueTypeFont(name, &face);
    return GlyphMapGenerator::Generate(face, chars, height);
  }

#define SCALE(Value) (Value * scale)
#define X(Pos) (pos[0])
#define Y(Pos) (pos[1])

  void Glyph::Draw(const glm::vec2 pos, const float scale, FontMesh& mesh) {
    float w = SCALE(size[0]);
    float h = SCALE(size[1]);
    float vertices[6][4] = {
      { X(pos), Y(pos) + h,           0.0f, 0.0f },
      { X(pos), Y(pos),               0.0f, 1.0f },
      { X(pos) + w, Y(pos),           1.0f, 1.0f },

      { X(pos), Y(pos) + h,           0.0f, 0.0f },
      { X(pos) + w, Y(pos),           1.0f, 1.0f },
      { X(pos) + w, Y(pos) + h,       1.0f, 0.0f },
    };
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
}