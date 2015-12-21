#include "skew_heap.h"
#include <iostream>
#include <stdexcept>
#include <climits>

int SkewHeap::KEY_INFINITY = INT_MAX;

SkewHeap::~SkewHeap()
{
}

SkewHeap::SkewHeap()
    : root(NodePtr(nullptr))
{}

SkewHeap::SkewHeap(const SkewHeap &other)
{
    root = makeNewPtr(other.root);
}

SkewHeap::SkewHeap(const int &key)
{
   root = NodePtr(new Node(key));
}

void SkewHeap::meld(IMergeableHeap<int> &otherBase)
{
    SkewHeap& other = dynamic_cast<SkewHeap &>(otherBase);
    root = merge(root, other.root);
    other.root = nullptr;
}

void SkewHeap::insert(const int &key)
{
    NodePtr tmp(std::move(makeTreePtr(key)));
    root = merge(root, tmp);
}

int SkewHeap::getMin() const
{
    if (!root)
        throw std::range_error("getting minimum from empty Skew Heap");
    return root->getKey();
}

int SkewHeap::extractMin()
{
    int min = this->getMin();
    NodePtr left(std::move(root->getLeft())),
            right(std::move(root->getRight()));
    root->getLeft() = NodePtr(nullptr);
    root->getRight() = NodePtr(nullptr);
    root = merge(left, right);
    return min;
}

SkewHeap::NodePtr SkewHeap::makeTreePtr(int key) const
{
    return std::move(NodePtr(new Node(key)));
}

size_t SkewHeap::size() const
{
    return root ? root->size_ : 0;
}

bool SkewHeap::empty() const
{
    return root == nullptr;
}

void SkewHeap::clear()
{
    root = NodePtr(nullptr);
}

SkewHeap::NodePtr SkewHeap::merge(NodePtr &A, NodePtr &B)
{
    if (!A)
        return std::move(B);
    if (!B)
        return std::move(A);
    if (A->getKey() > B->getKey()) {
        A.swap(B);
    }
    A->getRight() = merge(A->getRight(), B);
    if (needRotate(A)) {
        A->getRight().swap(A->getLeft());
    }
    recalc(A);
    return std::move(A);
}


bool SkewHeap::needRotate(const NodePtr &) const
{
    return true;
}

void SkewHeap::recalc(NodePtr &T) const
{
    if (!T)
        return;
    T->size_ = SkewTreeNode::size(T->getLeft()) + SkewTreeNode::size(T->getRight()) + 1;
}



SkewHeap::NodePtr& SkewHeap::SkewTreeNode::getLeft()
{
    return left_;
}

SkewHeap::NodePtr & SkewHeap::SkewTreeNode::getRight()
{
    return right_;
}

int& SkewHeap::SkewTreeNode::getKey()
{
    return key_;
}

SkewHeap::SkewTreeNode::SkewTreeNode()
    : left_(NodePtr(nullptr))
    , right_(NodePtr(nullptr))
    , key_(0)
    , size_(0)
{}

SkewHeap::SkewTreeNode::~SkewTreeNode()
{
}

SkewHeap::SkewTreeNode::SkewTreeNode(const int &key)
    : left_(NodePtr(nullptr))
    , right_(NodePtr(nullptr))
    , key_(key)
    , size_(1)
{}

SkewHeap::SkewTreeNode::SkewTreeNode(const SkewTreeNode &other)
    : key_(other.key_)
    , size_(other.size_)
{
    left_ = makeNewPtr(other.left_);
    right_ = makeNewPtr(other.right_);
}

size_t SkewHeap::SkewTreeNode::size(const SkewHeap::NodePtr &T)
{
    return T ? T->size_ : 0;
}

void SkewHeap::print(const SkewHeap::NodePtr &T, int d=0)
{
    if (!T)
        return;
    print(T->right_, d + 1);
    for (int i = 0; i < d; ++i)
        std::cout << "\t";
    std::cout << T->getKey() << std::endl;
    print(T->left_,d + 1);
}

void SkewHeap::print() const
{
    print(root);
}
