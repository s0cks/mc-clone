#ifndef MCC_IBO_BUILDER_H
#define MCC_IBO_BUILDER_H

#include "mcc/ibo/ibo_id.h"
#include "mcc/ibo/ibo_index.h"

namespace mcc::ibo {
  class Ibo;
  class IboBuilder {
  protected:
    IboBuilder() = default;
  public:
    virtual ~IboBuilder() = default;
    virtual Ibo* Build() const = 0;
  };

  template<typename Index>
  class IboBuilderTemplate : public IboBuilder {
  protected:
    typedef typename Index::Type IndexType;
    typedef typename Index::List IndexList;
  protected:
    IndexList data_;

    IboBuilderTemplate() = default;
    explicit IboBuilderTemplate(const IndexList& data):
      data_(data) {
    }
  public:
    ~IboBuilderTemplate() override = default;

    void Append(const IndexType value) {
      data_.push_back(value);
    }

    template<typename C>
    void Append(const C& values) {
      data_.insert(std::end(data_), std::begin(values), std::end(values));
    }
  };

  class UnsignedByteIboBuilder : public IboBuilderTemplate<UnsignedByteIndex> {
  public:
    UnsignedByteIboBuilder() = default;
    UnsignedByteIboBuilder(const IndexList& data):
      IboBuilderTemplate<UnsignedByteIndex>(data) {
    }
    ~UnsignedByteIboBuilder() override = default;

    void operator<<(const IndexType value) {
      return Append(value);
    }

    template<typename C>
    void operator<<(const C& values) {
      return Append<C>(values);
    }

    Ibo* Build() const override;
  };

  class UnsignedShortIboBuilder : public IboBuilderTemplate<UnsignedShortIndex> {
  public:
    UnsignedShortIboBuilder() = default;
    UnsignedShortIboBuilder(const IndexList& data):
      IboBuilderTemplate<UnsignedShortIndex>(data) {
    }
    ~UnsignedShortIboBuilder() override = default;

    void operator<<(const IndexType value) {
      return Append(value);
    }

    template<typename C>
    void operator<<(const C& values) {
      return Append<C>(values);
    }

    Ibo* Build() const override;
  };

  class UnsignedIntIboBuilder : public IboBuilderTemplate<UnsignedIntIndex> {
  public:
    UnsignedIntIboBuilder() = default;
    UnsignedIntIboBuilder(const IndexList& data):
      IboBuilderTemplate<UnsignedIntIndex>(data) {
    }
    ~UnsignedIntIboBuilder() override = default;

    void operator<<(const IndexType value) {
      return Append(value);
    }

    template<typename C>
    void operator<<(const C& values) {
      return Append<C>(values);
    }

    Ibo* Build() const override;
  };
}

#endif //MCC_IBO_BUILDER_H