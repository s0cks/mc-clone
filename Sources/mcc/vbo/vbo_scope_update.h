#ifndef MCC_VBO_SCOPE_H
#error "Please #include <mcc/vbo/vbo_scope.h> instead."
#endif //MCC_VBO_SCOPE_H

#ifndef MCC_VBO_SCOPE_UPDATE_H
#define MCC_VBO_SCOPE_UPDATE_H

#include "mcc/vbo/vbo_scope.h"

namespace mcc::vbo {
  class VboUpdateScope : public VboBindScope {
  protected:
    void UpdateVboLength(const uint64_t length);
  public:
    explicit VboUpdateScope(Vbo* vbo):
      VboBindScope(vbo) {
    }
    ~VboUpdateScope() override = default;

    void Update(const uint64_t offset, const uint8_t* bytes, const uint64_t num_bytes);

    void Update(const uint8_t* bytes, const uint64_t num_bytes) {
      return Update(0, bytes, num_bytes);
    }

    template<typename V>
    void Update(const std::vector<V>& vertices) {
      Update((const uint8_t*) &vertices[0], vertices.size() * sizeof(V));
      UpdateVboLength(vertices.size());
    }
  };
}

#endif //MCC_VBO_SCOPE_UPDATE_H