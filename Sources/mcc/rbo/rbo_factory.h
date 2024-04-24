#ifndef MCC_RBO_FACTORY_H
#define MCC_RBO_FACTORY_H

#include "mcc/rx.h"
#include "mcc/rbo/rbo_id.h"
#include "mcc/rbo/rbo_size.h"
#include "mcc/rbo/rbo_format.h"

namespace mcc::rbo {
  class Rbo;
  class RboFactory {
  protected:
    RboFormat format_;
    RboSize size_;

    virtual Rbo* CreateRbo(const RboId id) const;
  public:
    RboFactory(const RboFormat format,
               const RboSize& size):
      format_(format),
      size_(size) {
    }
    ~RboFactory() = default;
    virtual rx::observable<Rbo*> Create(const int num = 1) const;
  };
}

#endif //MCC_RBO_FACTORY_H