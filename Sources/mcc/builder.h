#ifndef MCC_BUILDER_H
#define MCC_BUILDER_H

#include "mcc/rx.h"
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

  template<class T>
  class BuilderTemplate : public BuilderBase {
  protected:
    BuilderTemplate() = default;
  public:
    ~BuilderTemplate() override = default;
    virtual T* Build() const = 0;
    virtual rx::observable<T*> BuildAsync() const = 0;
  };
}

#endif //MCC_BUILDER_H