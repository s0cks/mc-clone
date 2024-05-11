#include "mcc/ibo/ibo_builder.h"

namespace mcc::ibo {
  void IboBuilder::InitIbo(const IboId id) const {
    Ibo::BindIbo(id);
    Ibo::InitBufferData(GetData(), GetTotalSize(), GetUsage());
    Ibo::BindDefaultIbo();
  }

  UByteIbo* UByteIboBuilder::Build() const {
    const auto id = GenerateIboId();
    if(IsInvalidIboId(id)) {
      LOG(FATAL) << "failed to generate Ibo id.";
      return nullptr;
    }
    InitIbo(id);
    return UByteIbo::New(id, GetLength(), GetUsage());
  }

  UShortIbo* UShortIboBuilder::Build() const {
    const auto id = GenerateIboId();
    if(IsInvalidIboId(id)) {
      LOG(FATAL) << "failed to generate Ibo id.";
      return nullptr;
    }
    InitIbo(id);
    return UShortIbo::New(id, GetLength(), GetUsage());
  }

  UIntIbo* UIntIboBuilder::Build() const {
    const auto id = GenerateIboId();
    if(IsInvalidIboId(id)) {
      LOG(FATAL) << "failed to generate Ibo id.";
      return nullptr;
    }
    InitIbo(id);
    return UIntIbo::New(id, GetLength(), GetUsage());
  }
}