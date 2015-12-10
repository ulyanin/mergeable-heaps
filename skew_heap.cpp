#include "skew_heap.h"
#include <iostream>
#include <stdexcept>
#include <climits>

int SkewHeap::KEY_INFINITY = INT_MAX;

SkewHeap::~SkewHeap()
{
    delete root;
}

SkewHeap::SkewHeap()
    : root(nullptr)
{}

SkewHeap::SkewHeap(const SkewHeap &other)
{
    root = makeNewPtr(other.root);
}

SkewHeap::SkewHeap(const int &key)
{
   root = new Node(key);
}

void SkewHeap::meld(IMergeableHeap<int> &otherBase)
{
    SkewHeap& other = dynamic_cast<SkewHeap &>(otherBase);
    root = merge(root, other.root);
    other.root = nullptr;
}

void SkewHeap::insert(const int &key)
{
    root = merge(root, makeTreePtr(key));
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
    Node * left(root->getLeft()),
         * right(root->getRight());
    root->getLeft() = nullptr;
    root->getRight() = nullptr;
    delete root;
    root = merge(left, right);
    return min;
}

SkewHeap::Node * SkewHeap::makeTreePtr(int key) const
{
    return new Node(key);
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
    delete root;
    root = nullptr;
}

SkewHeap::Node * SkewHeap::merge(SkewHeap::Node *A, SkewHeap::Node *B)
{
    if (!A)
        return B;
    if (!B)
        return A;
    if (A->getKey() > B->getKey()) {
        std::swap(A, B);
    }
    A->getRight() = merge(A->getRight(), B);
    if (needRotate(A)) {
        std::swap(A->getRight(), A->getLeft());
    }
    recalc(A);
    return A;
}


bool SkewHeap::needRotate(SkewHeap::SkewTreeNode *) const
{
    return true;
}

void SkewHeap::recalc(Node * &T) const
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
    : left_(nullptr)
    , right_(nullptr)
    , key_(0)
    , size_(0)
{}

SkewHeap::SkewTreeNode::~SkewTreeNode()
{
    delete left_;
    delete right_;
}

SkewHeap::SkewTreeNode::SkewTreeNode(const int &key)
    : left_(nullptr)
    , right_(nullptr)
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

size_t SkewHeap::SkewTreeNode::size(SkewHeap::NodePtr T)
{
    return T ? T->size_ : 0;
}

void print(SkewHeap::Node * T, int d=0)
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
    ::print(root);
}
