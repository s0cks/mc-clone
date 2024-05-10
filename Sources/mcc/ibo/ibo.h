#ifndef MCC_INDEX_BUFFER_H
#define MCC_INDEX_BUFFER_H

#include <vector>
#include "mcc/gfx_target.h"

#include "mcc/gfx_usage.h"
#include "mcc/gfx_buffer_object.h"

#include "mcc/ibo/ibo_id.h"
#include "mcc/ibo/ibo_type.h"
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
    class Ibo : public gfx::BufferObjectTemplate<IboId> {
      friend class UByteIbo;
      friend class UShortIbo;
      friend class UIntIbo;
      friend class IboBuilder;
      
      template<typename I>
      friend class IboReadScope;
      friend class IboBindScope;
      friend class IboUpdateScope;
    public:
      static constexpr const auto kGlTarget = gfx::kIboTarget;
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
      static void InitBufferData(const uint8_t* bytes, const uint64_t num_bytes, const gfx::Usage usage);
      static void UpdateBufferData(const uint64_t offset, const uint8_t* bytes, const uint64_t num_bytes);
      
      static inline void
      UpdateBufferData(const uint8_t* bytes, const uint64_t num_bytes) {
        return UpdateBufferData(0, bytes, num_bytes);
      }

      static inline void
      InitBufferData(const uint64_t num_bytes, const gfx::Usage usage) {
        return InitBufferData(NULL, num_bytes, usage);
      }
      
      static inline void
      BindDefaultIbo() {
        return BindIbo(kInvalidIboId);
      }

      template<typename E, typename... Args>
      inline void
      Publish(Args... args) {
        return PublishEvent<E>(this, args...);
      }
    protected:
      Ibo(const IboId id,
          const uword elem_size,
          const uword length,
          const gfx::Usage usage):
        BufferObjectTemplate(id, elem_size, length, usage) {
      }

      void Destroy();
    public:
      ~Ibo() override = default;
      virtual GLenum GetType() const = 0;

      gfx::Usage GetUsage() const {
        return usage_;
      }

      inline bool HasDynamicUsage() const {
        return gfx::IsDynamicUsage(GetUsage());
      }

      inline bool HasStaticUsage() const {
        return gfx::IsStaticUsage(GetUsage());
      }

      inline bool HasStreamUsage() const {
        return gfx::IsStreamUsage(GetUsage());
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
                  const uword length,
                  const gfx::Usage usage):
        Ibo(id, T::GetSize(), length, usage) {
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
    };

    class UByteIbo : public IboTemplate<UnsignedByte> {
      friend class IboBuilder;
    protected:
      UByteIbo(const IboId id,
               const uword length,
               const gfx::Usage usage):
        IboTemplate(id, length, usage) {
      }
    public:
      ~UByteIbo() override = default;
      std::string ToString() const override;
    protected:
      static inline UByteIbo*
      New(const IboId id, const uword length, const gfx::Usage usage) {
        const auto ibo = new UByteIbo(id, length, usage);
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

      static UByteIbo* New(const uword num_indices, const gfx::Usage usage = gfx::kDefaultUsage);
      static UByteIbo* New(const IndexList& indices, const gfx::Usage usage = gfx::kDefaultUsage);

#define DEFINE_NEW_USAGE(Name, GlValue)                                   \
      static inline UByteIbo*                                             \
      New##Name(const IndexList& indices) {                               \
        return New(indices, gfx::k##Name##Usage);                         \
      }
      FOR_EACH_GFX_USAGE(DEFINE_NEW_USAGE)
#undef DEFINE_NEW_USAGE
    };

    class UShortIbo : public IboTemplate<UnsignedShort> {
      friend class IboBuilder;
    protected:
      UShortIbo(const IboId id,
                const uword length,
                const gfx::Usage usage):
        IboTemplate(id, length, usage) {
      }
    public:
      ~UShortIbo() override = default;
      std::string ToString() const override;
    protected:
      static inline UShortIbo*
      New(const IboId id, const uword length, const gfx::Usage usage) {
        const auto ibo = new UShortIbo(id, length, usage);
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

      static UShortIbo* New(const uword num_indices, const gfx::Usage usage = gfx::kDefaultUsage);
      static UShortIbo* New(const IndexList& indices, const gfx::Usage usage = gfx::kDefaultUsage);

#define DEFINE_NEW_USAGE(Name, GlValue)                                   \
      static inline UShortIbo*                                            \
      New##Name(const IndexList& indices) {                               \
        return New(indices, gfx::k##Name##Usage);                         \
      }
      FOR_EACH_GFX_USAGE(DEFINE_NEW_USAGE)
#undef DEFINE_NEW_USAGE
    };

    class UIntIbo : public IboTemplate<UnsignedInt> {
      friend class IboBuilder;
    private:
      UIntIbo(const IboId id,
              const uword length,
              const gfx::Usage usage):
        IboTemplate(id, length, usage) {
      }
    public:
      ~UIntIbo() override = default;
      std::string ToString() const override;
    private:
      static inline UIntIbo*
      New(const IboId id, const uword length, const gfx::Usage usage) {
        const auto ibo = new UIntIbo(id, length, usage);
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

      static UIntIbo* New(const uword num_indices, const gfx::Usage usage = gfx::kDefaultUsage);
      static UIntIbo* New(const IndexList& indices, const gfx::Usage usage = gfx::kDefaultUsage);

#define DEFINE_NEW_USAGE(Name, GlValue)                                   \
      static inline UIntIbo*                                              \
      New##Name(const IndexList& indices) {                               \
        return New(indices, gfx::k##Name##Usage);                         \
      }
      FOR_EACH_GFX_USAGE(DEFINE_NEW_USAGE)
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