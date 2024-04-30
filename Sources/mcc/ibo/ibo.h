#ifndef MCC_INDEX_BUFFER_H
#define MCC_INDEX_BUFFER_H

#include <vector>
#include "mcc/buffer_object.h"

#include "mcc/ibo/ibo_id.h"
#include "mcc/ibo/ibo_type.h"
#include "mcc/ibo/ibo_usage.h"
#include "mcc/ibo/ibo_events.h"
#include "mcc/ibo/ibo_registry.h"

namespace mcc {
  namespace ibo {
    class Ibo;
    rx::observable<IboEvent*> OnIboEvent();
    rx::observable<Ibo*> GetRegisteredIbos();

#define DEFINE_ON_IBO_EVENT(Name)                                 \
    static inline rx::observable<Name##Event*>                    \
    On##Name##Event() {                                           \
      return OnIboEvent()                                         \
        .filter(Name##Event::Filter)                              \
        .map(Name##Event::Cast);                                  \
    }                                                             \
    static inline rx::observable<Name##Event*>                    \
    On##Name##Event(const IboId id) {                             \
      return OnIboEvent()                                         \
        .filter(Name##Event::FilterBy(id))                        \
        .map(Name##Event::Cast);                                  \
    }
    FOR_EACH_IBO_EVENT(DEFINE_ON_IBO_EVENT)
#undef DEFINE_ON_IBO_EVENT

    class UByteIbo;
    class UShortIbo;
    class UIntIbo;
    class IboScope;
    class Ibo {
      friend class UByteIbo;
      friend class UShortIbo;
      friend class UIntIbo;
      friend class IboBuilder;
      
      friend class IboBindScope;
      friend class IboReadScope;
      friend class IboUpdateScope;
    public:
      struct IdComparator {
        bool operator() (const Ibo* lhs, const Ibo* rhs) const {
          return lhs->GetId() < rhs->GetId();
        }

        bool operator() (Ibo* lhs, Ibo* rhs) const {
          return lhs->GetId() < rhs->GetId();
        }
      };
    private:
      static void PublishEvent(IboEvent* event);

      template<typename E, typename... Args>
      static inline void
      PublishEvent(Ibo* ibo, Args... args) {
        E event(ibo, args...);
        return PublishEvent((IboEvent*) &event);
      }

      static void BindIbo(const IboId id);
      static void InitBufferData(const uint8_t* bytes, const uint64_t num_bytes, const Usage usage);
      static void UpdateBufferData(const uint64_t offset, const uint8_t* bytes, const uint64_t num_bytes);
      
      static inline void
      UpdateBufferData(const uint8_t* bytes, const uint64_t num_bytes) {
        return UpdateBufferData(0, bytes, num_bytes);
      }

      static inline void
      InitBufferData(const uint64_t num_bytes, const Usage usage) {
        return InitBufferData(NULL, num_bytes, usage);
      }
      
      static inline void
      BindDefaultIbo() {
        return BindIbo(kDefaultIboId);
      }

      template<typename E, typename... Args>
      inline void
      Publish(Args... args) {
        return PublishEvent<E>(this, args...);
      }
    protected:
      IboId id_;
      Usage usage_;
      uint64_t length_;
      
      Ibo(const IboId id,
          const Usage usage,
          const uint64_t length):
        id_(id),
        usage_(usage),
        length_(length) {
      }

      void Destroy();
    public:
      virtual ~Ibo() = default;
      virtual std::string ToString() const = 0;
      virtual uint64_t GetSize() const = 0;
      virtual GLenum GetType() const = 0;

      IboId GetId() const {
        return id_;
      }
      
      Usage GetUsage() const {
        return usage_;
      }

      uint64_t GetLength() const {
        return length_;
      }

      inline bool HasDynamicUsage() const {
        return IsDynamic(GetUsage());
      }

      inline bool HasStaticUsage() const {
        return IsStatic(GetUsage());
      }

      inline bool HasStreamUsage() const {
        return IsStream(GetUsage());
      }

      void Bind() const;
      void Unbind() const;
      void Update(const uint64_t offset, const uint8_t* data, const uint64_t len);

      inline void
      Update(const uint8_t* data, const uint64_t len) {
        return Update(0, data, len);
      }

      rx::observable<IboEvent*> OnEvent() const {
        return OnIboEvent()
          .filter([this](IboEvent* event) {
            return event
                && event->GetIboId() == GetId();
          });
      }

#define DEFINE_ON_IBO_EVENT(Name)                                             \
      rx::observable<Name##Event*> On##Name##Event() const {                  \
        return ibo::On##Name##Event(GetId());                                 \
      }
      FOR_EACH_IBO_EVENT(DEFINE_ON_IBO_EVENT)
#undef DEFINE_ON_IBO_EVENT
    };

    template<typename T>
    class IboTemplate : public Ibo {
    public:
      typedef typename T::Type Index;
      typedef std::vector<Index> IndexList;
      
      static inline uint64_t
      GetIndexSize() {
        return T::GetSize();
      }

      static inline constexpr const char*
      GetTypeName() {
        return T::GetName();
      }

      static inline constexpr const GLenum
      GetGlType() {
        return T::GetGlType();
      }

      static inline bool
      Filter(const Ibo* ibo) {
        return ibo
            && ibo->GetType() == GetGlType();
      }
    protected:
      IboTemplate(const IboId id,
                  const Usage usage,
                  const uint64_t length):
        Ibo(id, usage, length) {
      }
    public:
      ~IboTemplate() override = default;

      GLenum GetType() const override {
        return GetGlType();
      }

      inline void Update(const uint64_t offset, const Index* indices, const uint64_t num_indices) {
        return Update(offset, (const uint8_t*) indices, T::CalculateBufferSize(num_indices));
      }

      inline void Update(const uint64_t offset, const IndexList& indices) {
        return Update(offset, &indices[0], indices.size());
      }

      inline void Update(const Index* indices, const uint64_t num_indices) {
        return Update(0, indices, num_indices);
      }

      inline void Update(const IndexList& indices) {
        return Update(&indices[0], indices.size());
      }

      uint64_t GetSize() const override {
        return T::CalculateBufferSize(GetLength());
      }
    };

    class UByteIbo : public IboTemplate<UnsignedByte> {
      friend class IboBuilder;
    protected:
      UByteIbo(const IboId id,
               const Usage usage,
               const uint64_t length):
        IboTemplate(id, usage, length) {
      }
    public:
      ~UByteIbo() override = default;
      std::string ToString() const override;
    protected:
      static inline UByteIbo*
      New(const IboId id, const Usage usage, const uint64_t length) {
        const auto ibo = new UByteIbo(id, usage, length);
        MCC_ASSERT(ibo);
        PublishEvent<IboCreatedEvent>(ibo);
        return ibo;
      }
    public:
      static inline const UByteIbo*
      Cast(const Ibo* ibo) {
        MCC_ASSERT(ibo);
        MCC_ASSERT(Filter(ibo));
        return (const UByteIbo*) ibo;
      }

      static UByteIbo* New(const uint64_t num_indices, const Usage usage = kDefaultUsage);
      static UByteIbo* New(const IndexList& indices, const Usage usage = kDefaultUsage);

#define DEFINE_NEW_USAGE(Name, GlValue)                                   \
      static inline UByteIbo*                                             \
      New##Name(const IndexList& indices) {                               \
        return New(indices, k##Name);                                     \
      }
      FOR_EACH_IBO_USAGE(DEFINE_NEW_USAGE)
#undef DEFINE_NEW_USAGE
    };

    class UShortIbo : public IboTemplate<UnsignedShort> {
      friend class IboBuilder;
    protected:
      UShortIbo(const IboId id,
                const Usage usage,
                const uint64_t length):
        IboTemplate(id, usage, length) {
      }
    public:
      ~UShortIbo() override = default;
      std::string ToString() const override;
    protected:
      static inline UShortIbo*
      New(const IboId id, const Usage usage, const uint64_t length) {
        const auto ibo = new UShortIbo(id, usage, length);
        MCC_ASSERT(ibo);
        PublishEvent<IboCreatedEvent>(ibo);
        return ibo;
      }
    public:
      static inline const UShortIbo*
      Cast(const Ibo* ibo) {
        MCC_ASSERT(ibo);
        MCC_ASSERT(Filter(ibo));
        return (const UShortIbo*) ibo;
      }

      static UShortIbo* New(const uint64_t num_indices, const Usage usage = kDefaultUsage);
      static UShortIbo* New(const IndexList& indices, const Usage usage = kDefaultUsage);

#define DEFINE_NEW_USAGE(Name, GlValue)                                   \
      static inline UShortIbo*                                            \
      New##Name(const IndexList& indices) {                               \
        return New(indices, k##Name);                                     \
      }
      FOR_EACH_IBO_USAGE(DEFINE_NEW_USAGE)
#undef DEFINE_NEW_USAGE
    };

    class UIntIbo : public IboTemplate<UnsignedInt> {
      friend class IboBuilder;
    private:
      UIntIbo(const IboId id,
              const Usage usage,
              const uint64_t length):
        IboTemplate(id, usage, length) {
      }
    public:
      ~UIntIbo() override = default;
      std::string ToString() const override;
    private:
      static inline UIntIbo*
      New(const IboId id, const Usage usage, const uint64_t length) {
        const auto ibo = new UIntIbo(id, usage, length);
        MCC_ASSERT(ibo);
        PublishEvent<IboCreatedEvent>(ibo);
        return ibo;
      }
    public:
      static inline const UIntIbo*
      Cast(const Ibo* ibo) {
        MCC_ASSERT(ibo);
        MCC_ASSERT(Filter(ibo));
        return (const UIntIbo*) ibo;
      }

      static UIntIbo* New(const uint64_t num_indices, const Usage usage = kDefaultUsage);
      static UIntIbo* New(const IndexList& indices, const Usage usage = kDefaultUsage);

#define DEFINE_NEW_USAGE(Name, GlValue)                                   \
      static inline UIntIbo*                                              \
      New##Name(const IndexList& indices) {                               \
        return New(indices, k##Name);                                     \
      }
      FOR_EACH_IBO_USAGE(DEFINE_NEW_USAGE)
#undef DEFINE_NEW_USAGE
    };

    const IboRegistry& GetRegistry();
  }
  using ibo::Ibo;
  using ibo::UByteIbo;
  using ibo::UShortIbo;
  using ibo::UIntIbo;
}

#endif //MCC_INDEX_BUFFER_H