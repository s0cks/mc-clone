#include "mcc/mesh/vertex.h"
#include "mcc/font/font_renderer.h"

namespace mcc::font {
  void FontRenderer::RenderText(const std::string& text, const glm::vec2 pos) {
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader()->ApplyShader();
    shader()->SetVec3("textColor", color_);
    glActiveTexture(GL_TEXTURE0);

    float startX = pos[0];
    float startY = pos[1];
    float x = startX;

    glBindVertexArray(mesh().vao);
    std::string::const_iterator iter;
    for(iter = text.begin(); iter != text.end(); iter++) {
      auto ch = chars()[(*iter)];
      const glm::vec2 pos = {
        scale(x + ch.bearing[0]),
        scale(startY - (ch.size[1] - ch.bearing[1])),
      };
      ch.Draw(pos, scale_, mesh());
      x += scale(ch.advance >> 6);
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
  }
}