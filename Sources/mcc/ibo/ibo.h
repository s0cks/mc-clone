#ifndef MCC_INDEX_BUFFER_H
#define MCC_INDEX_BUFFER_H

#include <vector>
#include "mcc/buffer_object.h"

#include "mcc/ibo/ibo_id.h"
#include "mcc/ibo/ibo_type.h"
#include "mcc/ibo/ibo_usage.h"
#include "mcc/ibo/ibo_events.h"

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

    class IboScope;
    class Ibo {
      friend class IboScope;
    public:
      struct IdComparator {
        bool operator() (Ibo* lhs, Ibo* rhs) const {
          return lhs->GetId() < rhs->GetId();
        }
      };

      enum DrawMode : GLenum {
        kPoints = GL_POINTS,
        kLineStrip = GL_LINE_STRIP,
        kLineLoop = GL_LINE_LOOP,
        kLines = GL_LINES,
        kLineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
        kLinesAdjacency = GL_LINES_ADJACENCY,
        kTriangleStrip = GL_TRIANGLE_STRIP,
        kTriangleFan = GL_TRIANGLE_FAN,
        kTriangles = GL_TRIANGLES,
        kTriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
        kTrianglesAdjacency = GL_TRIANGLES_ADJACENCY,
        kPatches = GL_PATCHES,

        kNumberOfDrawModes = 12,
      };
    private:
      static void Publish(IboEvent* event);

      template<typename E, typename... Args>
      static void
      Publish(Args... args) {
        E event(args...);
        return Publish((IboEvent*) &event);
      }

      static void BindIbo(const IboId id);
      
      static inline void
      BindDefaultIbo() {
        return BindIbo(kDefaultIboId);
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
        Publish<IboCreatedEvent>(GetId());
      }

      void Destroy();
    public:
      virtual ~Ibo() {
        Publish<IboDestroyedEvent>(GetId());
      }
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

      static inline constexpr const char*
      GetTypeName() {
        return T::GetName();
      }

      static inline constexpr const GLenum
      GetGlType() {
        return T::GetGlType();
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
        return new UByteIbo(id, usage, length);
      }
    public:
      static UByteIbo* New(const Index* indices, const uint64_t num_indices, const Usage usage);
      
      static inline UByteIbo*
      New(const IndexList& indices, const Usage usage) {
        return New(&indices[0], indices.size(), usage);
      }

#define DEFINE_NEW_USAGE(Name, GlValue)                                   \
      static inline UByteIbo*                                              \
      New##Name(const Index* indices, const uint64_t num_indices) {       \
        return New(indices, num_indices, k##Name);                        \
      }                                                                   \
      static inline UByteIbo*                                              \
      New##Name(const IndexList& indices) {                               \
        return New(indices, k##Name);                                     \
      }
      FOR_EACH_IBO_USAGE(DEFINE_NEW_USAGE)
#undef DEFINE_NEW_USAGE
    };

    class UShortIbo : public IboTemplate<UnsignedShort> {
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
        return new UShortIbo(id, usage, length);
      }
    public:
      static UShortIbo* New(const Index* indices, const uint64_t num_indices, const Usage usage);
      
      static inline UShortIbo*
      New(const IndexList& indices, const Usage usage) {
        return New(&indices[0], indices.size(), usage);
      }

#define DEFINE_NEW_USAGE(Name, GlValue)                                   \
      static inline UShortIbo*                                              \
      New##Name(const Index* indices, const uint64_t num_indices) {       \
        return New(indices, num_indices, k##Name);                        \
      }                                                                   \
      static inline UShortIbo*                                              \
      New##Name(const IndexList& indices) {                               \
        return New(indices, k##Name);                                     \
      }
      FOR_EACH_IBO_USAGE(DEFINE_NEW_USAGE)
#undef DEFINE_NEW_USAGE
    };

    class UIntIbo : public IboTemplate<UnsignedInt> {
    private:
      UIntIbo(const IboId id,
              const Usage usage,
              const uint64_t length):
        IboTemplate(id, usage, length) {
      }
    public:
      ~UIntIbo() override = default;
      std::string ToString() const override;
    protected:
      static inline UIntIbo*
      New(const IboId id, const Usage usage, const uint64_t length) {
        return new UIntIbo(id, usage, length);
      }
    public:
      static UIntIbo* New(const Index* indices, const uint64_t num_indices, const Usage usage);
      
      static inline UIntIbo*
      New(const IndexList& indices, const Usage usage) {
        return New(&indices[0], indices.size(), usage);
      }

#define DEFINE_NEW_USAGE(Name, GlValue)                                   \
      static inline UIntIbo*                                              \
      New##Name(const Index* indices, const uint64_t num_indices) {       \
        return New(indices, num_indices, k##Name);                        \
      }                                                                   \
      static inline UIntIbo*                                              \
      New##Name(const IndexList& indices) {                               \
        return New(indices, k##Name);                                     \
      }
      FOR_EACH_IBO_USAGE(DEFINE_NEW_USAGE)
#undef DEFINE_NEW_USAGE
    };

    static inline rx::observable<IboId>
    GetRegisteredIboIds() {
      return GetRegisteredIbos()
        .map([](Ibo* ibo) {
          return ibo->GetId();
        });
    }
  }
  using ibo::Ibo;
  using ibo::UByteIbo;
  using ibo::UShortIbo;
  using ibo::UIntIbo;
}

#endif //MCC_INDEX_BUFFER_H