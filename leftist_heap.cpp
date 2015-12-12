#include "leftist_heap.h"
#include <iostream>

LeftistHeap::~LeftistHeap()
{}

LeftistHeap::LeftistHeap()
    : SkewHeap()
{}

LeftistHeap::LeftistHeap(const int &key)
{
    root = new LeftistTreeNode(key);
}

LeftistHeap::LeftistHeap(const LeftistHeap &other)
    : SkewHeap(other)
{}

LeftistHeap::LeftistTreeNode * LeftistHeap::castNode(SkewHeap::Node * T) const
{
    return dynamic_cast<LeftistTreeNode *>(T);
}

size_t LeftistHeap::getRang(SkewHeap::Node *T) const
{
    if (!T)
        return 0;
    return castNode(T)->getRang();
}

void LeftistHeap::recalc(SkewHeap::Node * &T) const
{
    if (!T)
        return;
    //std::cout << "recalc Leftist" << std::endl;
    SkewHeap::recalc(T);
    castNode(T)->getRang() = std::min(getRang(T->getLeft()),
                                      getRang(T->getRight())) + 1;
}

bool LeftistHeap::needRotate(const SkewHeap::Node * T) const
{
    if (!T)
        return 0;
    return getRang(T->left_) < getRang(T->right_);
}


SkewHeap::Node * LeftistHeap::makeTreePtr(int key) const
{
    return new LeftistTreeNode(key);
}

LeftistHeap::LeftistTreeNode::~LeftistTreeNode()
{}

LeftistHeap::LeftistTreeNode::LeftistTreeNode()
    : SkewHeap::SkewTreeNode()
    , rang_(1)
{}

LeftistHeap::LeftistTreeNode::LeftistTreeNode(const int &key)
    : SkewHeap::SkewTreeNode(key)
    , rang_(1)
{}

LeftistHeap::LeftistTreeNode::LeftistTreeNode(const LeftistTreeNode &other)
    : SkewHeap::SkewTreeNode(other)
    , rang_(other.rang_)
{}

size_t & LeftistHeap::LeftistTreeNode::getRang()
{
    return rang_;
}

