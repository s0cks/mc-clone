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

      void InitIbo(const IboId id) const;
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
      virtual T* Build() const = 0;
      
      virtual rx::observable<T*> BuildAsync() const {
        return rx::observable<>::create<T*>([this](rx::subscriber<T*> s) {
          const auto value = Build();
          if(!value) {
            const auto err = "failed to build Ibo.";
            return s.on_error(rx::util::make_error_ptr(std::runtime_error(err)));
          }
          s.on_next(value);
          s.on_completed();
        });
      }

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
      UByteIbo* Build() const override;
    };

    class UShortIboBuilder : public IboBuilderTemplate<UShortIbo> {
    public:
      explicit UShortIboBuilder(const uint64_t length = 0):
        IboBuilderTemplate<UShortIbo>(length) {
      }
      ~UShortIboBuilder() override = default;
      UShortIbo* Build() const override;
    };

    class UIntIboBuilder : public IboBuilderTemplate<UIntIbo> {
    public:
      explicit UIntIboBuilder(const uint64_t length = 0):
        IboBuilderTemplate<UIntIbo>(length) {   
      }
      ~UIntIboBuilder() override = default;
      UIntIbo* Build() const override;
    };
  }
}

#endif //MCC_IBO_BUILDER_H