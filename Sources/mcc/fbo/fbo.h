#ifndef MCC_FRAMEBUFFER_H
#define MCC_FRAMEBUFFER_H

#include "mcc/fbo/fbo_id.h"
#include "mcc/fbo/fbo_events.h"
#include "mcc/fbo/fbo_attachment.h"

namespace mcc {
  namespace fbo {
    rx::observable<FboEvent*> OnFboEvent();
    rx::observable<FboEvent*> OnFboEvent(const FboId id);

#define DEFINE_ON_FBO_EVENT(Name)                                   \
    static inline rx::observable<Name##Event*>                      \
    On##Name##Event() {                                             \
      return OnFboEvent()                                           \
        .filter(Name##Event::Filter)                                \
        .map(Name##Event::Cast);                                    \
    }                                                               \
    static inline rx::observable<Name##Event*>                      \
    On##Name##Event(const FboId id) {                               \
      return OnFboEvent()                                           \
        .filter(Name##Event::FilterBy(id))                          \
        .map(Name##Event::Cast);                                    \
    }
    FOR_EACH_FBO_EVENT(DEFINE_ON_FBO_EVENT)
#undef DEFINE_ON_FBO_EVENT

    class FboFactory;
    class Fbo {
      friend class FboFactory;
      friend class ColorAttachment;
    private:
      static void PublishEvent(FboEvent* event);

      template<typename E, typename... Args>
      static inline void
      PublishEvent(Fbo* fbo, Args... args) {
        E event(fbo, args...);
        return PublishEvent(&event);
      }

      static void BindFbo(const FboId id);
      static void DeleteFbos(const FboId* ids, const int num_ids);

      static void AttachTexture(const FboTarget target,
                                const AttachmentPoint p,
                                const texture::TextureTarget textarget,
                                const texture::TextureId texid,
                                const int level = 0);

      static inline void
      AttachTexture2D(const FboTarget target,
                      const AttachmentPoint p,
                      const texture::TextureId texid,
                      const int level = 0) {
        return AttachTexture(target, p, texture::k2D, texid, level);
      }

      static inline void
      BindDefaultFbo() {
        return BindFbo(kDefaultFboId);
      }

      static inline void
      DeleteFbo(const FboId id) {
        return DeleteFbos(&id, 1);
      }
    protected:
      FboId id_;

      virtual void Destroy();
    private:
      explicit Fbo(const FboId id):
        id_(id) {
      }
    public:
      virtual ~Fbo() = default;
      virtual std::string ToString() const;

      FboId GetId() const {
        return id_;
      }

      inline rx::observable<FboEvent*>
      OnEvent() const {
        return fbo::OnFboEvent(GetId());
      }

#define DEFINE_ON_FBO_EVENT(Name)               \
      inline rx::observable<Name##Event*>       \
      On##Name##Event() const {                 \
        return fbo::On##Name##Event(GetId());   \
      }
      FOR_EACH_FBO_EVENT(DEFINE_ON_FBO_EVENT)
#undef DEFINE_ON_FBO_EVENT
    private:
      static Fbo* New(const FboId id);
    public:
      static Fbo* New();
    };
  }
  using fbo::Fbo;
}

#endif //MCC_FRAMEBUFFER_H