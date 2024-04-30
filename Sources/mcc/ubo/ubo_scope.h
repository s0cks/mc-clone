#ifndef MCC_UBO_SCOPE_H
#define MCC_UBO_SCOPE_H

#include "mcc/ubo/ubo.h"

namespace mcc::ubo {
  class UboScope {
  protected:
    Ubo* ubo_;

    explicit UboScope(Ubo* ubo):
      ubo_(ubo) {
    }
  public:
    virtual ~UboScope() = default;

    Ubo* GetUbo() const {
      return ubo_;
    }

    UboId GetUboId() const {
      return GetUbo()->GetId();
    }
  };

  class UboBindScope : public UboScope {
  public:
    explicit UboBindScope(Ubo* ubo):
      UboScope(ubo) {
      Bind();
    }
    ~UboBindScope() override {
      Unbind();
    }

    void Bind() {
      Ubo::BindUbo(GetUboId());
    }

    void Unbind() {
      Ubo::UnbindUbo();
    }
  };

  class UboUpdateScope : public UboBindScope {
  public:
    explicit UboUpdateScope(Ubo* ubo):
      UboBindScope(ubo) {
    }
    ~UboUpdateScope() override = default;

    void Update(const uint64_t offset, const uint8_t* bytes, const uint64_t num_bytes) {
      Ubo::UpdateUbo(offset, bytes, num_bytes);
    }

    inline void Update(const uint8_t* bytes, const uint64_t num_bytes) {
      return Update(0, bytes, num_bytes);
    }
  };

  template<typename T>
  class UboReadScope : public UboBindScope {
  protected:
    uword start_;
    uword end_;
  public:
    explicit UboReadScope(Ubo* ubo):
      UboBindScope(ubo),
      start_(0),
      end_(0) {
      const auto map = glMapBuffer(Ubo::kGlTarget, GL_READ_ONLY);
      if(!map) {
        LOG(ERROR) << "failed to map gl buffer:";
        CHECK_GL(FATAL);
        return;
      }

      start_ = (uword) map;
      end_ = start_ + GetUbo()->GetTotalSize();
    }
    ~UboReadScope() override {
      if(IsOpen()) {
        glUnmapBuffer(Ubo::kGlTarget);
        CHECK_GL(FATAL);
        start_ = end_ = 0;
      }
    }

    uword GetStartingAddress() const {
      return start_;
    }

    bool HasStartingAddress() const {
      return start_ > 0;
    }

    uword GetEndingAddress() const {
      return end_;
    }

    bool HasEndingAddress() const {
      return end_ > 0;
    }

    bool IsOpen() const {
      return HasStartingAddress()
          && GetEndingAddress() > GetStartingAddress();
    }

    rx::observable<T> ReadAll() const {
      if(!IsOpen())
        return rx::observable<>::empty<T>();
      return rx::observable<>::create<T>([this](rx::subscriber<T> s) {
        auto current = GetStartingAddress();
        while(current < GetEndingAddress()) {
          const auto v = (T*)current;
          s.on_next(*v);
          current += sizeof(T);
        }
        s.on_completed();
      });
    }
  };
}

#endif //MCC_UBO_SCOPE_H