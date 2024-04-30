#ifndef MCC_VBO_SCOPE_H
#error "Please #include <mcc/vbo/vbo_scope.h> instead."
#endif //MCC_VBO_SCOPE_H

#ifndef MCC_VBO_SCOPE_READ_H
#define MCC_VBO_SCOPE_READ_H

#include "mcc/vbo/vbo_scope.h"

namespace mcc::vbo {
  template<class V>
  class VboReadScope : public VboBindScope {
  protected:
    uword read_start_;
    uword read_end_;
  public:
    explicit VboReadScope(Vbo* vbo):
      VboBindScope(vbo),
      read_start_(0),
      read_end_(0) {
      const auto map = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
      if(map) {
        read_start_ = (uword) map;
        read_end_ = read_start_ + GetVbo()->GetSize();
      }
    }
    ~VboReadScope() override {
      if(read_start_ != 0) {
        glUnmapBuffer(GL_ARRAY_BUFFER);
        CHECK_GL(FATAL);
      }
    }

    rx::observable<V> ReadAll() const {
      if(read_start_ == 0)
        return rx::observable<>::empty<V>();
      return rx::observable<>::create<V>([this](rx::subscriber<V> s) {
        auto curr = read_start_;
        while(curr < read_end_) {
          const auto v = (V*)curr;
          s.on_next(*v);
          curr += sizeof(V);
        }
        s.on_completed();
      });
    }
  };
}

#endif //MCC_VBO_SCOPE_READ_H