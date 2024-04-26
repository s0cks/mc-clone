#include "mcc/ibo/ibo_builder.h"

namespace mcc::ibo {
  template<class IboType>
  IboType* IboBuilder::InitIbo(const IboId id) const {
    Ibo::BindIbo(id);
    Ibo::InitBufferData(GetData(), GetTotalSize(), GetUsage());
    Ibo::BindDefaultIbo();
    return IboType::New(id, GetUsage(), GetLength());
  }

  rx::observable<UByteIbo*> UByteIboBuilder::Build(const int num) const {
    return GenerateIboId(num)
      .map([this](const IboId id) {
        return InitIbo<UByteIbo>(id);
      });
  }

  rx::observable<UShortIbo*> UShortIboBuilder::Build(const int num) const {
    return GenerateIboId(num)
      .map([this](const IboId id) {
        return InitIbo<UShortIbo>(id);
      });
  }

  rx::observable<UIntIbo*> UIntIboBuilder::Build(const int num) const {
    return GenerateIboId(num)
      .map([this](const IboId id) {
        return InitIbo<UIntIbo>(id);
      });
  }
}