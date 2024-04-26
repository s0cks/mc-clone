#ifndef MCC_IBO_BUILDER_H
#define MCC_IBO_BUILDER_H

#include "mcc/rx.h"
#include "mcc/common.h"
#include "mcc/ibo/ibo.h"

namespace mcc {
  namespace ibo {
    class IboBuilder {
    protected:
      Usage usage_;

      IboBuilder():
        usage_(kDefaultUsage) {
      }

      template<class IboType>
      IboType* InitIbo(const IboId id) const;
    public:
      virtual ~IboBuilder() = default;
      virtual const uint8_t* GetData() const = 0;
      virtual uint64_t GetLength() const = 0;
      virtual uint64_t GetIndexSize() const = 0;

      void SetUsage(const Usage& rhs) {
        usage_ = rhs;
      }

      virtual Usage GetUsage() const {
        return usage_;
      }
      
      virtual uint64_t GetTotalSize() const {
        return GetLength() * GetIndexSize();
      }
    };

    template<class T>
    class IboBuilderTemplate : public IboBuilder {
    protected:
      typename T::IndexList data_;
    public:
      ~IboBuilderTemplate() override = default;
      virtual rx::observable<T*> Build() const = 0;

      uint64_t GetIndexSize() const override {
        return T::GetIndexSize();
      }

      uint64_t GetLength() const override {
        return data_.size();
      }

      const uint8_t* GetData() const override {
        return data_.empty()
            ? NULL
            : (const uint8_t*) &data_[0];
      }
    };

    class UByteIboBuilder : public IboBuilderTemplate<UByteIbo> {
    public:
      UByteIboBuilder() = default;
      ~UByteIboBuilder() override = default;
      rx::observable<UByteIbo*> Build() const override;
    };

    class UShortIboBuilder : public IboBuilderTemplate<UShortIbo> {
    public:
      UShortIboBuilder() = default;
      ~UShortIboBuilder() override = default;
      rx::observable<UShortIbo*> Build() const override;
    };

    class UIntIboBuilder : public IboBuilderTemplate<UIntIbo> {
    public:
      UIntIboBuilder() = default;
      ~UIntIboBuilder() override = default;
      rx::observable<UIntIbo*> Build() const override;
    };
  }
}

#endif //MCC_IBO_BUILDER_H