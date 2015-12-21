#ifndef SKEW_HEAP_H
#define SKEW_HEAP_H

#include "imergeable_heap.h"
#include <memory>

class SkewHeap : public IMergeableHeap<int>
{
protected:
    class SkewTreeNode;
public:
    virtual ~SkewHeap();
    SkewHeap();
    SkewHeap(const SkewHeap &);
    SkewHeap(const int &);
    virtual void meld(IMergeableHeap<int> &);
    virtual void insert(const int &);
    virtual int extractMin();
    virtual int getMin() const;
    virtual size_t size() const;
    virtual bool empty() const;
    virtual void clear();
    virtual void print() const;
    //size_t getRang(Node *) const;
    static int KEY_INFINITY;
protected:
    typedef SkewTreeNode Node;
    typedef std::unique_ptr<Node> NodePtr;
    NodePtr root;
    virtual NodePtr merge(NodePtr &A, NodePtr &B);
    virtual void recalc(NodePtr &) const;
    virtual bool needRotate(const NodePtr &) const;
    virtual NodePtr makeTreePtr(int) const;
    static void print(const NodePtr &, int);
};

template <class T>
std::unique_ptr<T> makeNewPtr(const std::unique_ptr<T> &ptr)
{
    if (!ptr)
        return std::move(std::unique_ptr<T>(nullptr));
    return std::move(std::unique_ptr<T>(new T(*ptr)));
}

class SkewHeap::SkewTreeNode
{
    friend class SkewHeap;
public:
    SkewTreeNode();
    virtual ~SkewTreeNode();
    SkewTreeNode(const int &);
    SkewTreeNode(const SkewTreeNode &);
    static size_t size(const NodePtr &);
    virtual NodePtr & getLeft();
    virtual NodePtr & getRight();
    virtual int& getKey();
    NodePtr left_,
            right_;
    int key_;
    size_t size_;
};



#endif
