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

  typedef GLuint VertexArrayObjectId;
  static constexpr const VertexArrayObjectId kInvalidVertexArrayObject = 0;
  class VertexArrayObject {
  protected:
    VertexArrayObjectId id_;
  public:
    explicit VertexArrayObject(const VertexArrayObjectId id = kInvalidVertexArrayObject):
      id_(id) {
    }
    VertexArrayObject(const VertexArrayObject& rhs):
      id_(rhs.id_) {
    }
    ~VertexArrayObject() = default;
  };


  typedef GLuint VertexBufferObjectId;
  static constexpr VertexBufferObjectId kInvalidVertexBufferObject = 0;

  class VertexBufferObject {
  protected:
    VertexBufferObjectId id_;
  public:
    explicit VertexBufferObject(const VertexBufferObjectId id = kInvalidVertexBufferObject):
      id_(id) {
    }
    VertexBufferObject(const VertexBufferObject& rhs):
      id_(rhs.id_) {
    }
    virtual ~VertexBufferObject() = default;

    VertexBufferObjectId id() const {
      return id_;
    }

    void Bind() {
      glBindBuffer(GL_ARRAY_BUFFER, id_);
      CHECK_GL(FATAL);
    }

    void Unbind() {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      CHECK_GL(FATAL);
    }

    void Delete() {
      glDeleteBuffers(1, &id_);
      CHECK_GL(FATAL);
    }

    void operator=(const VertexBufferObject& rhs) {
      id_ = rhs.id_;
    }

    void operator=(const VertexBufferObjectId& rhs) {
      id_ = rhs;
    }

    friend std::ostream& operator<<(std::ostream& stream, const VertexBufferObject& rhs) {
      stream << "VertexBufferObject(";
      stream << "id=" << rhs.id_;
      stream << ")";
      return stream;
    }
  };

  class GuiVertexBufferObject : public VertexBufferObject {
  public:
    explicit GuiVertexBufferObject(const VertexBufferObjectId id = kInvalidVertexBufferObject):
      VertexBufferObject(id) {
    }
    explicit GuiVertexBufferObject(const VertexList& vertices):
      VertexBufferObject() {
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