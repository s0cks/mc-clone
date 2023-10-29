#ifndef MCC_GUI_VERTEX_H
#define MCC_GUI_VERTEX_H

#include <vector>
#include "mcc/gfx.h"

namespace mcc::gui {
  struct Vertex {
    glm::vec2 pos;
    glm::vec2 uv;
    glm::vec3 color;
  };

  typedef std::vector<Vertex> VertexList;

  class GuiVertexBufferObject : public VertexBufferObject {
  public:
    explicit GuiVertexBufferObject(const VertexBufferObjectId id = kInvalidVertexBufferObject):
      VertexBufferObject(id) {
    }
    explicit GuiVertexBufferObject(const VertexList& vertices):
      VertexBufferObject(kInvalidVertexBufferObject) {
      glGenBuffers(1, &id_);
      glBindBuffer(GL_ARRAY_BUFFER, id_);
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) 0);
      glEnableVertexAttribArray(0);

      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, uv));
      glEnableVertexAttribArray(1);

      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*) offsetof(Vertex, color));
      glEnableVertexAttribArray(2);   
    }
    ~GuiVertexBufferObject() override = default;
  };
}

#endif //MCC_GUI_VERTEX_H