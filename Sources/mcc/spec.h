#ifndef MCC_SPEC_H
#define MCC_SPEC_H

namespace mcc {
  class Spec {
  protected:
    Spec() = default;
  public:
    virtual ~Spec() = default;
    virtual const char* GetName() const = 0;
  };

#define DEFINE_SPEC(Name)                                  \
  public:                                                  \
    const char* GetName() const override { return #Name; }

#define DECLARE_SPEC_PROPERTY(Name, Type) \
  virtual std::optional<Type> Get##Name() const = 0;
}

#endif //MCC_SPEC_H