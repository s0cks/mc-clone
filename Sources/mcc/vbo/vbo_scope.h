#ifndef MCC_VBO_SCOPE_H
#define MCC_VBO_SCOPE_H

#include "mcc/vbo/vbo.h"

namespace mcc {
  namespace vbo {
    class VboScope {
    protected:
      const Vbo* vbo_;
      
      explicit VboScope(const Vbo* vbo):
        vbo_(vbo) {
        MCC_ASSERT(vbo);
      }
    public:
      virtual ~VboScope() = default;

      const Vbo* GetVbo() const {
        return vbo_;
      }

      VboId GetVboId() const {
        return GetVbo()->GetId();
      }
    };

    class VboBindScope : public VboScope {
    public:
      explicit VboBindScope(const Vbo* vbo):
        VboScope(vbo) {
        Bind();
      }
      ~VboBindScope() override {
        Unbind();
      }

      void Bind() {
        Vbo::BindVbo(GetVboId());
      }

      void Unbind() {
        Vbo::BindDefaultVbo();
      }
    };

    class VboDrawScope : public VboBindScope {
    public:
      explicit VboDrawScope(const Vbo* vbo):
        VboBindScope(vbo) {
      }
      ~VboDrawScope() override = default;
    };
  }
}

#endif //MCC_VBO_SCOPE_H