#ifndef MCC_IBO_BUILDER_H
#define MCC_IBO_BUILDER_H

#include "mcc/rx.h"
#include "mcc/common.h"
#include "mcc/ibo/ibo.h"

namespace mcc {
  namespace ibo {
    class IboBuilder {
    protected:
      gfx::Usage usage_;

      IboBuilder():
        usage_(gfx::kDefaultUsage) {
      }

      template<class IboType>
      IboType* InitIbo(const IboId id) const;
    public:
      virtual ~IboBuilder() = default;
      virtual const uint8_t* GetData() const = 0;
      virtual uint64_t GetLength() const = 0;
      virtual uint64_t GetIndexSize() const = 0;

      void SetUsage(const gfx::Usage& rhs) {
        usage_ = rhs;
      }

      virtual gfx::Usage GetUsage() const {
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
      uint64_t length_;

      explicit IboBuilderTemplate(const uint64_t length = 0):
        IboBuilder(),
        data_(),
        length_(length) {
      }
    public:
      ~IboBuilderTemplate() override = default;
      virtual rx::observable<T*> Build(const int num = 1) const = 0;

      uint64_t GetIndexSize() const override {
        return T::GetIndexSize();
      }

      uint64_t GetLength() const override {
        return data_.empty()
             ? length_
             : data_.size();
      }

      const uint8_t* GetData() const override {
        return data_.empty()
            ? NULL
            : (const uint8_t*) &data_[0];
      }

      void Append(const typename T::Index value) {
        data_.push_back(value);
      }

      template<typename C>
      void Append(const C& values) {
        data_.insert(std::end(data_), std::begin(values), std::end(values));
      }
    };

    class UByteIboBuilder : public IboBuilderTemplate<UByteIbo> {
    public:
      explicit UByteIboBuilder(const uint64_t length = 0):
        IboBuilderTemplate<UByteIbo>(length) {
      }
      ~UByteIboBuilder() override = default;
      rx::observable<UByteIbo*> Build(const int num = 1) const override;
    };

    class UShortIboBuilder : public IboBuilderTemplate<UShortIbo> {
    public:
      explicit UShortIboBuilder(const uint64_t length = 0):
        IboBuilderTemplate<UShortIbo>(length) {
      }
      ~UShortIboBuilder() override = default;
      rx::observable<UShortIbo*> Build(const int num = 1) const override;
    };

    class UIntIboBuilder : public IboBuilderTemplate<UIntIbo> {
    public:
      explicit UIntIboBuilder(const uint64_t length = 0):
        IboBuilderTemplate<UIntIbo>(length) {   
      }
      ~UIntIboBuilder() override = default;
      rx::observable<UIntIbo*> Build(const int num = 1) const override;
    };
  }
}

#endif //MCC_IBO_BUILDER_H