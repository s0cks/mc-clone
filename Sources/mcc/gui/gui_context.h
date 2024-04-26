#ifndef MCC_GUI_CONTEXT_H
#define MCC_GUI_CONTEXT_H

#include "mcc/glm.h"
#include "mcc/color.h"
#include "mcc/common.h"

#include "mcc/vao/vao.h"
#include "mcc/ibo/ibo.h"
#include "mcc/vbo/vbo.h"
#include "mcc/vbo/vbo_attribute.h"

namespace mcc::gui {
  struct Vertex {
    glm::vec2 pos;
    Color color;
    
    Vertex() = default;
    Vertex(const Vertex& rhs) = default;
    ~Vertex() = default;

    Vertex& operator=(const Vertex& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const Vertex& rhs) {
      stream << "gui::Vertex(";
      stream << "pos=" << glm::to_string(rhs.pos) << ", ";
      stream << "color=" << glm::to_string(rhs.color);
      stream << ")";
      return stream;
    }
  };

  typedef std::vector<Vertex> VertexList;

  class PosAttr : public vbo::Vec2fAttribute<0, offsetof(Vertex, pos), sizeof(Vertex)>{};
  class ColorAttr : public vbo::ColorAttribute<1, offsetof(Vertex, color), sizeof(Vertex)>{};

  class Context {
    friend class ContextScope;
  protected:
    Context(const int32_t num_vertices, const int32_t num_indices);
  public:
    virtual ~Context();
  private:
    static void SetCurrent(Context* ctx);

    static inline void
    ClearCurrent() {
      return SetCurrent(nullptr);
    }
  public:
    static Context* GetCurrent();

    static inline bool
    HasCurrent() {
      return GetCurrent() != nullptr;
    }

    static inline bool
    IsOpen() {
      return !HasCurrent();
    }
  };

  class ContextScope {
  protected:
    Context* ctx_;
  public:
    explicit ContextScope(Context* ctx):
      ctx_(ctx) {
      MCC_ASSERT(ctx);
      Enter();
    }
    ~ContextScope() {
      Exit();
    }

    Context* GetContext() const {
      return ctx_;
    }

    void Enter() {
      MCC_ASSERT(Context::IsOpen());
      Context::SetCurrent(GetContext());
    }

    void Exit() {
      MCC_ASSERT(Context::HasCurrent());
      Context::ClearCurrent();
    }
  };
}

#endif //MCC_GUI_CONTEXT_H