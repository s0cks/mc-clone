#ifndef MCC_IBO_FACTORY_H
#define MCC_IBO_FACTORY_H

#include <vector>

#include "mcc/rx.h"
#include "mcc/ibo/ibo_id.h"
#include "mcc/ibo/ibo_type.h"
#include "mcc/ibo/ibo_usage.h"

namespace mcc::ibo {
  class IboFactoryBase {
  protected:
    static void BindBuffer(const IboId id);
    static void PutBufferData(const IboId id, const uint8_t* data, const uint64_t size, const Usage usage);

    static inline void
    BindDefaultBuffer() {
      return BindBuffer(kDefaultIboId);
    }
  protected:
    const uint8_t* data_;
    const uint64_t size_;

    IboFactoryBase(const uint8_t* data,
                   const uint64_t size):
      data_(data),
      size_(size) {
    }

    inline const uint8_t* data() const {
      return data_;
    }

    inline uint64_t size() const {
      return size_;
    }
  public:
    virtual ~IboFactoryBase() = default;

    rx::observable<IboId> Create(const Usage usage, const int num = 1) const {
      DLOG(INFO) << "creating " << num << " ibos w/ " << usage << " usage...";
      return GenerateIboId(num)
          .map([this,usage](const IboId id) {
            DLOG(INFO) << "initializing ibo #" << id << " w/ " << size() << " bytes....";
            BindBuffer(id);
            PutBufferData(id, data(), size(), usage);
            BindDefaultBuffer();
            return id;
          });
    }
  };

  template<class T>
  class IboFactory : public IboFactoryBase {
  public:
    typedef typename T::Type Index;
    typedef typename std::vector<Index> IndexList;
  public:
    IboFactory(const Index* indices, const uint64_t num_indices):
      IboFactoryBase((const uint8_t*) indices, T::CalculateBufferSize(num_indices)) {
    }
    virtual ~IboFactory() = default;
  };
}

#endif //MCC_IBO_FACTORY_H