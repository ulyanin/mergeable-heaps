#ifndef LEFTIST_HEAP_H
#define LEFTIST_HEAP_H

#include "skew_heap.h"

class LeftistHeap : public SkewHeap
{
    class LeftistTreeNode;
public:
    virtual ~LeftistHeap();
    LeftistHeap();
    LeftistHeap(const LeftistHeap &);
    LeftistHeap(const int &);
protected:
    LeftistTreeNode * castNode(Node *) const;
    size_t getRang(const NodePtr &) const;
    virtual void recalc(NodePtr &) const;
    virtual bool needRotate(const NodePtr &) const;
    virtual SkewHeap::NodePtr makeTreePtr(int) const;
};

class LeftistHeap::LeftistTreeNode : public SkewHeap::SkewTreeNode
{
public:
    ~LeftistTreeNode();
    LeftistTreeNode();
    LeftistTreeNode(const int &);
    LeftistTreeNode(const LeftistTreeNode &);
    size_t& getRang();
protected:
    size_t rang_;
};


#endif
