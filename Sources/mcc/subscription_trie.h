#ifndef MCC_CALLBACK_TRIE_H
#define MCC_CALLBACK_TRIE_H

#include "mcc/trie.h"

namespace mcc::trie {
  template<typename Subscription, typename Callback>
  class SubscriptionNode : public trie::SequenceItemNode {
  protected:
    Subscription subscription_;
    Callback callback_;
  public:
    explicit SubscriptionNode(const Subscription& subscription, Callback callback):
      trie::SequenceItemNode(),
      subscription_(subscription),
      callback_(callback) {
    }
    ~SubscriptionNode() override = default;

    Subscription GetSubscription() const {
      return subscription_;
    }

    Callback GetCallback() const {
      return callback_;
    }

    friend bool operator==(const SubscriptionNode& lhs, const Subscription& rhs) {
      return lhs.subscription_ == rhs;
    }

    friend bool operator!=(const SubscriptionNode& lhs, const Subscription& rhs) {
      return lhs.subscription_ != rhs;
    }
  };

  template<typename C>
  class SubscriptionSequenceNode : public trie::SequenceNode {
  protected:
    explicit SubscriptionSequenceNode():
      trie::SequenceNode() {
    }
  public:
    ~SubscriptionSequenceNode() override = default;

    void Register(const Mouse::Subscription& subscription, C callback) {
      Append(new trie::SubscriptionNode<Mouse::Subscription, C>(subscription, callback));
    }

    void Deregister(const Mouse::Subscription& subscription) {
      const auto head = (trie::SubscriptionNode<Mouse::Subscription, C>*)head_;
      if(head == nullptr)
        return;

      if(head->GetSubscription() == subscription) {
        head_ = head->GetNext();
        delete head_;
        return;
      }

      SequenceNodeIterator iter(this);
      while(iter.HasNext()) {
        const auto node = (trie::SubscriptionNode<Mouse::Subscription, C>*)iter.Next();
        if(node->GetSubscription() == subscription) {
          const auto prev = node->GetPrevious();
          const auto next = node->GetNext();
          if(prev)
            prev->SetNext(next);
          if(next)
            next->SetPrevious(prev);
          delete node;
          break;
        }
      }
    }
  };
}

#endif //MCC_CALLBACK_TRIE_H