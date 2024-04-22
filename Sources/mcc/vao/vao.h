#ifndef MCC_VAO_H
#define MCC_VAO_H

#include "mcc/vao/vao_id.h"

namespace mcc {
  namespace vao {
    class Vao {
    protected:
      VaoId id_;

      explicit Vao(const VaoId id):
        id_(id) {
      }
    public:
      ~Vao() = default;

      VaoId GetId() const {
        return id_;
      }

      inline bool IsValid() const {
        return IsValidVaoId(GetId());
      }

      inline bool IsDefault() const {
        return IsDefaultVaoId(GetId());
      }
    public:
      static inline Vao*
      New(const VaoId id) {
        MCC_ASSERT(IsValidVaoId(id));
        return new Vao(id);
      }

      static inline Vao*
      New() {
        const auto id = GenerateVaoId(1)
          .as_blocking()
          .first();
        if(!IsValidVaoId(id))
          return nullptr;
        return New(id);
      }
    };
  }
  using vao::Vao;
}

#endif //MCC_VAO_H