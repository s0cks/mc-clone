#ifndef MCC_ENTITY_TRACKER_H
#define MCC_ENTITY_TRACKER_H

#include "mcc/entity/entity_id.h"
#include "mcc/entity/entity_set.h"
#include "mcc/entity/entity_events.h"
#include "mcc/entity/entity_signature.h"

namespace mcc::entity {
  class EntityVisitor;
  class EntityTracker {
  protected:
    rx::subscription on_created_;
    rx::subscription on_destroyed_;
    EntitySet entities_;

    virtual void OnCreated(const EntityId id) {
      entities_.Put(id);
    }

    virtual void OnDestroyed(const EntityId id) {
      entities_.Remove(id);
    }
  public:
    EntityTracker();

    virtual ~EntityTracker() {
      on_created_.unsubscribe();
      on_destroyed_.unsubscribe();
    }

    virtual bool Contains(const EntityId id) const {
      return entities_.Contains(id);
    }

    virtual bool VisitAll(std::function<bool(EntityId)> vis) const;

    explicit operator rx::observable<EntityId> () const {
      return (rx::observable<EntityId>) entities_;
    }
  };

  class FilteredEntityTracker : public EntityTracker {
  protected:
    Signature signature_;
    rx::subscription on_signature_changed_;

    void OnCreated(const EntityId id) override { }

    inline bool
    HasSignature(const Signature& sig) {
      return (sig & signature_) == signature_;
    }

    virtual void OnSignatureChanged(const EntityId id, const Signature& new_sig) {
      if(HasSignature(new_sig)) {
        entities_.Put(id);
        return;
      }
      entities_.Remove(id);
      return;
    }

    void OnDestroyed(const EntityId id) override { }
  public:
    FilteredEntityTracker();
    explicit FilteredEntityTracker(const Signature& signature):
      FilteredEntityTracker() {
      SetSignature(signature);
    }
    ~FilteredEntityTracker() override {
      on_signature_changed_.unsubscribe();
    }

    void SetSignature(const Signature& signature);

    explicit operator rx::observable<EntityId> () const {
      return (rx::observable<EntityId>) entities_;
    }
  };
}

#endif //MCC_ENTITY_TRACKER_H