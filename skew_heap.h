#ifndef SKEW_HEAP_H
#define SKEW_HEAP_H

#include "imergeable_heap.h"

class SkewHeap : public IMergeableHeap<int>
{
protected:
    class SkewTreeNode;
public:
    typedef SkewTreeNode Node;
    typedef SkewTreeNode * NodePtr;
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
    Node * root;
    virtual Node * merge(Node * A, Node * B);
    virtual void recalc(Node * &) const;
    virtual bool needRotate(Node *) const;
    virtual Node * makeTreePtr(int) const;
};

template <class T>
T * makeNewPtr(T * ptr)
{
    if (!ptr)
        return ptr;
    return new T(*ptr);
}

class SkewHeap::SkewTreeNode
{
public:
    SkewTreeNode();
    virtual ~SkewTreeNode();
    SkewTreeNode(const int &);
    SkewTreeNode(const SkewTreeNode &);
    friend size_t size(SkewTreeNode *);
    virtual SkewTreeNode *& getLeft();
    virtual SkewTreeNode *& getRight();
    virtual int& getKey();
    Node * left_,
         * right_;
    int key_;
    size_t size_;
};

size_t size(SkewHeap::Node *);

void print(SkewHeap::Node *, int);

#endif
