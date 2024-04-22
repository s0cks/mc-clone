#ifndef MCC_INDEX_BUFFER_H
#define MCC_INDEX_BUFFER_H

#include "mcc/buffer_object.h"

#include "mcc/ibo/ibo_id.h"
#include "mcc/ibo/ibo_events.h"

namespace mcc {
  class IndexBufferObject : public BufferObjectTemplate<kIndex> {
  public:
    explicit IndexBufferObject(const BufferObjectId id):
      BufferObjectTemplate(id) {
    }
    IndexBufferObject() = default;
    IndexBufferObject(const IndexBufferObject& rhs):
      BufferObjectTemplate(rhs) {  
    }
    ~IndexBufferObject() override = default;
    virtual GLenum type() const = 0;
    virtual uint64_t length() const = 0;
    virtual uint64_t index_size() const = 0;

    virtual uint64_t size() const {
      return length() * index_size();
    }
  };

  template<typename Index, const GLenum Type, const GlObjectUsage Usage = kDefaultUsage>
  class IndexBufferTemplate : public IndexBufferObject {
  public:
    static const uint64_t kIndexSize = sizeof(Index);
    typedef std::vector<Index> IndexList;
  protected:
    uint64_t length_;
  public:
    IndexBufferTemplate() = default;
    explicit IndexBufferTemplate(const BufferObjectId id):
      IndexBufferObject(id) {
    }
    explicit IndexBufferTemplate(const Index* indices,
                                 const uint64_t num_indices):
      IndexBufferObject(),
      length_(num_indices) {
      BufferData(indices, num_indices);
    }
    explicit IndexBufferTemplate(const uint64_t num_indices):
      IndexBufferObject(),
      length_(num_indices) {
      BindBufferData(num_indices);
    }
    explicit IndexBufferTemplate(const IndexList& indices):
      IndexBufferTemplate(&indices[0], indices.size()) {
    }
    ~IndexBufferTemplate() override = default;

    void BufferData(const Index* indices, const uint64_t num_indices) {
      DLOG_IF(ERROR, num_indices == 0) << "creating IndexBufferObject w/ 0 indices.";
      glBufferData(target(), num_indices * kIndexSize, &indices[0], Usage);
      CHECK_GL(FATAL);
    }

    void BindBufferData(const Index* indices, const uint64_t num_indices) {
      Bind();
      BufferData(indices, num_indices);
    }

    void BufferData(const uint64_t size) {
      DLOG_IF(ERROR, size == 0) << "creating IndexBufferObject w/ size " << size;
      glBufferData(target(), size, NULL, Usage);
      CHECK_GL(FATAL);
    }

    void BindBufferData(const uint64_t size) {
      Bind();
      BufferData(size);
    }

    GlObjectUsage usage() const override {
      return Usage;
    }

    uint64_t length() const override {
      return length_;
    }

    uint64_t index_size() const override {
      return kIndexSize;
    }

    GLenum type() const override {
      return Type;
    }

    void operator=(const IndexBufferTemplate<Index, Usage>& rhs) {
      BufferObject::operator=((const BufferObject&) rhs);
      length_ = rhs.length_;
    }
  };

  namespace u8 {
    typedef GLubyte Index;
    typedef std::vector<Index> IndexList;
    typedef IndexBufferTemplate<Index, GL_UNSIGNED_BYTE> IndexBuffer;
  }

  namespace u16 {
    typedef GLushort Index;
    typedef std::vector<Index> IndexList;
    typedef IndexBufferTemplate<Index, GL_UNSIGNED_SHORT> IndexBuffer;
  }

  namespace u32 {
    typedef GLuint Index;
    typedef std::vector<Index> IndexList;
    typedef IndexBufferTemplate<Index, GL_UNSIGNED_INT> IndexBuffer;
  }
}

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

    class Ibo {
    public:
      struct IdComparator {
        bool operator() (Ibo* lhs, Ibo* rhs) const {
          return lhs->GetId() < rhs->GetId();
        }
      };
    protected:
      IboId id_;
      
      explicit Ibo(const IboId id):
        id_(id) {
      }
    public:
      ~Ibo() = default;

      IboId GetId() const {
        return id_;
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

      std::string ToString() const;
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
}

#endif //MCC_INDEX_BUFFER_H