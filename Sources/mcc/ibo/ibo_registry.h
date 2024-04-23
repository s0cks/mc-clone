#ifndef MCC_IBO_REGISTRY_H
#define MCC_IBO_REGISTRY_H

#include <set>
#include "mcc/ibo/ibo_events.h"

namespace mcc::ibo {
  class Ibo;
  class UByteIbo;
  class UShortIbo;
  class UIntIbo;
  class IboRegistry {
  public:
    struct IdComparator {
      bool operator() (const Ibo* lhs, const Ibo* rhs) const;
    };

    typedef std::set<const Ibo*, IdComparator> IboSet;
  private:
    IboSet ibos_;
    rx::subscription on_created_sub_;
    rx::subscription on_destroyed_sub_;

    void Register(const Ibo* ibo);
    void Deregister(const Ibo* ibo);
  public:
    explicit IboRegistry(const rx::observable<IboEvent*>& events);
    virtual ~IboRegistry();

    inline rx::observable<const Ibo*> GetAllIbos() const {
      return rx::observable<>::iterate(ibos_);
    }

    virtual rx::observable<const UByteIbo*> GetAllUByteIbos() const;
    virtual rx::observable<const UShortIbo*> GetAllUShortIbos() const;
    virtual rx::observable<const UIntIbo*> GetAllUIntIbos() const;
  };
}

#endif //MCC_IBO_REGISTRY_H