#ifndef MCC_BUILDER_H
#define MCC_BUILDER_H

#include "mcc/metadata.h"

namespace mcc {
  class BuilderBase {
  protected:
    Metadata meta_;
  public:
    virtual ~BuilderBase() = default;

    const Metadata& GetMeta() const {
      return meta_;
    }

    void SetMeta(const Metadata& rhs) {
      meta_ = rhs;
    }

    void SetName(const std::string& name) {
      return meta_.SetName(name);
    }

    void Append(const Tag& rhs) {
      return meta_.Append(rhs);
    }

    void Append(const TagSet& rhs) {
      return meta_.Append(rhs);
    }

    void Append(const TagList& rhs) {
      return meta_.Append(rhs);
    }
  };
}

#endif //MCC_BUILDER_H