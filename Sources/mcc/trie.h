#ifndef MCC_TRIE_H
#define MCC_TRIE_H

#include <cstdint>

namespace mcc::trie {
  class Node {
  protected:
    Node* parent_;

    Node():
      parent_(nullptr) {
    }
  public:
    virtual ~Node() = default;

    virtual Node* GetParent() const {
      return parent_;
    }

    virtual void SetParent(Node* parent) {
      parent_ = parent;
    }

    virtual bool HasParent() const {
      return parent_ != nullptr;
    }
    
    virtual bool IsRoot() const {
      return parent_ == nullptr;
    }

    virtual bool IsLeaf() const {
      return false;
    }

    virtual uint64_t GetNumberOfChildren() const {
      return 0;
    }

    virtual Node* GetChildAt(const uint64_t idx) const {
      return nullptr;
    }

    virtual bool HasChildAt(const uint64_t idx) const {
      return false;
    }

    virtual void SetChildAt(const uint64_t idx, Node* node) {
      // do nothing
    }

    virtual bool HasChildren() const {
      return false;
    }
  };

  template<const uint64_t NumberOfChildren>
  class EdgeNode : public Node {
  protected:
    Node* children_[NumberOfChildren];

    EdgeNode():
      Node(),
      children_() {
    }
  public:
    ~EdgeNode() override = default;

    bool IsLeaf() const override {
      return NumberOfChildren == 0;
    }
    
    uint64_t GetNumberOfChildren() const override {
      return NumberOfChildren;
    }

    Node* GetChildAt(const uint64_t idx) const override {
      return children_[idx];
    }

    bool HasChildAt(const uint64_t idx) const override {
      return children_[idx] != nullptr;
    }

    void SetChildAt(const uint64_t idx, Node* node) override {
      children_[idx] = node;
    }

    bool HasChildren() const override {
      return NumberOfChildren > 0;
    }
  };

  class SequenceItemNode : public Node {
  protected:
    SequenceItemNode* next_;
    SequenceItemNode* previous_;

    SequenceItemNode() = default;
  public:
    ~SequenceItemNode() override = default;

    SequenceItemNode* GetNext() const {
      return next_;
    }

    void SetNext(SequenceItemNode* node) {
      next_ = node;
    }

    bool HasNext() const {
      return next_;
    }

    SequenceItemNode* GetPrevious() const {
      return previous_;
    }

    void SetPrevious(SequenceItemNode* node) {
      previous_ = node;
    }

    bool HasPrevious() const {
      return previous_ != nullptr;
    }
  };

  class SequenceNode : public Node {
  public:
    class SequenceNodeIterator {
    protected:
      SequenceItemNode* next_;
    public:
      SequenceNodeIterator(const SequenceNode* node):
        next_(node->head_) {
      }
      ~SequenceNodeIterator() = default;

      bool HasNext() const {
        return next_ != nullptr;
      }

      SequenceItemNode* Next() {
        auto next = next_;
        next_ = next->GetNext();
        return next;
      }
    };
  protected:
    SequenceItemNode* head_;
    SequenceItemNode* tail_;

    explicit SequenceNode():
      Node(),
      head_(nullptr),
      tail_(nullptr) {
    }
  public:
    ~SequenceNode() override = default;

    SequenceItemNode* GetHead() const {
      return head_;
    }

    void SetHead(SequenceItemNode* node) {
      head_ = node;
    }

    bool HasHead() const {
      return head_ != nullptr;
    }

    SequenceItemNode* GetTail() const {
      return tail_;
    }

    void SetTail(SequenceItemNode* node) {
      tail_ = node;
    }

    bool HasTail() const {
      return tail_ != nullptr;
    }

    void Append(SequenceItemNode* node) {
      if(head_ == nullptr)
        head_ = node;
      InsertAfter(tail_, node);
      tail_ = node;
    }

    void InsertAfter(SequenceItemNode* prev, SequenceItemNode* node) {
      if(prev == nullptr || node == nullptr)
        return;
      node->SetNext(prev->GetNext());
      prev->SetNext(node);
    }
  };
}

#endif //MCC_TRIE_H