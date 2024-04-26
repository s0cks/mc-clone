#ifndef MCC_VBO_SCOPE_H
#define MCC_VBO_SCOPE_H

#include "mcc/vbo/vbo.h"

namespace mcc {
  namespace vbo {
    class VboScope {
    protected:
      Vbo* vbo_;
      
      explicit VboScope(Vbo* vbo):
        vbo_(vbo) {
        MCC_ASSERT(vbo);
      }
    public:
      virtual ~VboScope() = default;

      Vbo* GetVbo() const {
        return vbo_;
      }

      VboId GetVboId() const {
        return GetVbo()->GetId();
      }
    };

    class VboBindScope : public VboScope {
    public:
      explicit VboBindScope(Vbo* vbo):
        VboScope(vbo) {
        Bind();
      }
      ~VboBindScope() override {
        Unbind();
      }

      void Bind() {
        Vbo::BindVbo(GetVboId());
      }

      void Unbind() {
        Vbo::BindDefaultVbo();
      }
    };

    class VboUpdateScope : public VboBindScope {
    public:
      explicit VboUpdateScope(Vbo* vbo):
        VboBindScope(vbo) {
      }
      ~VboUpdateScope() override = default;

      void Update(const uint64_t offset, const uint8_t* bytes, const uint64_t num_bytes) {
        Vbo::UpdateVboData(offset, bytes, num_bytes);
      }

      void Update(const uint8_t* bytes, const uint64_t num_bytes) {
        return Update(0, bytes, num_bytes);
      }

      template<typename V>
      void Update(const std::vector<V>& vertices) {
        Update((const uint8_t*) &vertices[0], vertices.size() * sizeof(V));
        GetVbo()->SetLength(vertices.size());
      }
    };

    template<class V>
    class VboReadScope : public VboBindScope {
    protected:
      uword read_start_;
      uword read_end_;
    public:
      explicit VboReadScope(Vbo* vbo):
        VboBindScope(vbo),
        read_start_(0),
        read_end_(0) {
        const auto map = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
        if(map) {
          read_start_ = (uword) map;
          read_end_ = read_start_ + GetVbo()->GetSize();
        }
      }
      ~VboReadScope() override {
        if(read_start_ != 0) {
          glUnmapBuffer(GL_ARRAY_BUFFER);
          CHECK_GL(FATAL);
        }
      }

      rx::observable<V> ReadAll() const {
        if(read_start_ == 0)
          return rx::observable<>::empty<V>();
        return rx::observable<>::create<V>([this](rx::subscriber<V> s) {
          auto curr = read_start_;
          while(curr < read_end_) {
            const auto v = (V*)curr;
            s.on_next(*v);
            curr += sizeof(V);
          }
          s.on_completed();
        });
      }
    };

    class VboDrawScope : public VboBindScope {
    public:
      explicit VboDrawScope(Vbo* vbo):
        VboBindScope(vbo) {
      }
      ~VboDrawScope() override = default;
      
      void Draw(const GLenum mode, const int64_t first, const int64_t count) const;

      inline void Draw(const GLenum mode, const int64_t first = 0) const { 
        return Draw(mode, first, GetVbo()->GetLength());
      }
    };
  }
}

#endif //MCC_VBO_SCOPE_H