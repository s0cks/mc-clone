#include "mcc/vao/vao_builder.h"
#include "mcc/vao/vao.h"

namespace mcc::vao {
  Vao* VaoBuilder::Build() const {
    const auto id = GenerateVaoId();
    if(IsInvalidVaoId(id)) {
      DLOG(ERROR) << "failed to generate VaoId.";
      return nullptr;
    }
    return Vao::New(meta_, id);
  }

  rx::observable<Vao*> VaoBuilder::BuildAsync() const {
    return rx::observable<>::create<Vao*>([this](rx::subscriber<Vao*> s) {
      const auto vao = Build();
      if(!vao) {
        const auto err = rx::util::make_error_ptr("Failed to create Vao.");
        return s.on_error(err);
      }
      s.on_next(vao);
      s.on_completed();
    });
  }
}