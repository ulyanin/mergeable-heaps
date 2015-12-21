#include "binomial_heap.h"
#include <functional>
#include <algorithm>
#include <iostream>
#include <climits>

const int BinomialHeap::KEY_INFINITY = INT_MAX;

inline size_t BinomialHeap::Node::childrenSize(const BinomialHeap::NodePtr &T)
{
    if (T == nullptr)
        return 0;
    return T->children_.size();
}

inline int BinomialHeap::Node::getKey(const NodePtr &T)
{
    if (T == nullptr)
        return BinomialHeap::KEY_INFINITY;
    return T->key_;
}


BinomialHeap::NodePtr BinomialHeap::Node::mergeTree(NodePtr &A, NodePtr &B)
{
    if (A == nullptr)
        return std::move(B);
    if (B == nullptr)
        return std::move(A);
    if (A->key_ > B->key_) {
        A.swap(B);
    }
    A->children_.push_back(std::move(B));
    B = nullptr;
    return std::move(A);
}

void BinomialHeap::Node::print(const NodePtr &T, int d=0)
{
    if (!T)
        return;
    for (int i = 0; i < d; ++i)
        std::cout << "\t";
    std::cout << T->key_ << std::endl;
    for (size_t deg = 0; deg < childrenSize(T); ++deg) {
        print(T->children_[deg], d + 1);
    }
}

/*
 * this function merges trees A, B and addtitional like bits, saves result in A and returns it
 *   0 + 0 = 0 and additional is 0
 *   0 + 1 = 1 + 0 = 1 and additional is 0
 *   1 + 1 = 0 and additional is 1
 * and then do not forget about additional
 */
void BinomialHeap::Node::mergeSaveDegree(NodePtr &A, NodePtr &B, NodePtr &additional, size_t deg)
{
    A = mergeTree(A, B);
    if (childrenSize(A) != deg)
    {
        /*
         * if merging A and B returns "0" (with overflow or not)
         * so if
         * A == B == 0 then
         *      A = addditinal;
         *      additional = B = 0
         * A == B == 1 then
         *      A = additional;
         *      additinal = 1;
         *      B = 0;
         */
        A.swap(additional);
    } else {
        /*
         * So A != 0 and there was not overflow
         * we just merge others
         */
        A = mergeTree(A, additional);
        if (childrenSize(A) != deg) {
            /*
             * if overflow, push this to additinal
             */
            A.swap(additional);
        }
    }
}

BinomialHeap::Node::Node(int key)
    : key_(key)
{}

BinomialHeap::Node::Node(const Node &other)
    : key_(other.key_)
{
    children_.clear();
    children_.resize(other.children_.size());
    for (size_t deg = 0; deg < other.children_.size(); ++deg)
    {
        children_[deg] = std::move(BinomialHeap::makeNewNode(other.children_[deg]));
    }
}

BinomialHeap::Node::~Node()
{
    children_.clear();
}

void BinomialHeap::print() const
{
    std::cout << "-------------" << std::endl;
    for (size_t deg = 0; deg < this->maxDegree(); ++deg) {
        Node::print(trees_[deg]);
        std::cout << "____________\n";
    }
    std::cout << "-------------" << std::endl;
}

BinomialHeap::NodePtr BinomialHeap::makeNewNode(const NodePtr &other)
{
    if (!other)
        return std::move(NodePtr(nullptr));
    return std::move(NodePtr(new Node(*other)));
}

void BinomialHeap::meld(IMergeableHeap<int> &otherBase)
{
    BinomialHeap & other = dynamic_cast<BinomialHeap &>(otherBase);
    NodePtr overflow(nullptr);
    trees_.resize(std::max(this->maxDegree(), other.maxDegree()));
    other.trees_.resize(std::max(this->maxDegree(), other.maxDegree()));
    for (size_t deg = 0; deg < other.maxDegree(); ++deg) {
        Node::mergeSaveDegree(trees_[deg], other.trees_[deg], overflow, deg);
    }
    size_ += other.size_;
    other.clear();
    if (overflow != nullptr) {
        trees_.push_back(std::move(overflow));
    }
    min_ = std::min(min_, other.min_);
    //this->print();
}

BinomialHeap::BinomialHeap()
    : size_(0)
    , min_(BinomialHeap::KEY_INFINITY)
{}

BinomialHeap::BinomialHeap(const int &key)
    : size_(1)
    , min_(key)
{
    trees_.push_back(std::move(NodePtr(new BinomialHeap::Node(key))));
}

BinomialHeap::BinomialHeap(const BinomialHeap &other)
    : size_(other.size_)
    , min_(other.min_)
{
    trees_.resize(other.maxDegree());
    for (size_t deg = 0; deg < other.maxDegree(); ++deg)
    {
        trees_[deg] = std::move(makeNewNode(other.trees_[deg]));
    }
}

BinomialHeap::BinomialHeap(NodePtr &T)
    : size_(Node::childrenSize(T))
    , min_(BinomialHeap::KEY_INFINITY)
{
    if (T) {
        trees_.push_back(std::move(T));
        min_ = T->key_;
    }
}

BinomialHeap::BinomialHeap(std::vector<NodePtr> &otherTrees)
    : size_((1 << otherTrees.size()) - 1)
{
    trees_.swap(otherTrees);
    updateMin();
}

void BinomialHeap::updateMin()
{
    min_ = BinomialHeap::KEY_INFINITY;
    for (auto &ptr : trees_) {
        if (ptr != nullptr) {
            min_ = std::min(min_, Node::getKey(ptr));
        }
    }
}

size_t BinomialHeap::maxDegree() const
{
    return trees_.size();
}

void BinomialHeap::insert(const int &key)
{
    BinomialHeap tmp(key);
    this->meld(tmp);
}

void BinomialHeap::clear()
{
    trees_.clear();
    size_ = 0;
    min_ = BinomialHeap::KEY_INFINITY;
}


BinomialHeap::~BinomialHeap()
{
}

int BinomialHeap::getMin() const
{
    if (this->trees_.back() == nullptr)
        throw std::runtime_error("last element of heap is empty");
    int min = BinomialHeap::KEY_INFINITY;
    for (auto &ptr : trees_) {
        if (ptr != nullptr) {
            min = std::min(min, Node::getKey(ptr));
        }
    }
    return min;
}

size_t BinomialHeap::size() const
{
    return size_;
}

bool BinomialHeap::empty() const
{
    return trees_.empty();
}

void BinomialHeap::shrink()
{
    while (!trees_.empty() && trees_.back() == nullptr)
        trees_.pop_back();
}

int BinomialHeap::extractMin()
{
    if (trees_.size() == 0)
        throw std::range_error("erase from empty heap");
    int minTreeIndex = trees_.size() - 1;
    int min = Node::getKey(trees_[minTreeIndex]);
    for (size_t deg = 0; deg < trees_.size(); ++deg) {
        if (Node::getKey(trees_[deg]) < min) {
            minTreeIndex = deg;
            min = Node::getKey(trees_[deg]);
        }
    }
    size_t was = size_;
    NodePtr minTree(std::move(trees_[minTreeIndex]));
    trees_[minTreeIndex] = nullptr;
    BinomialHeap tmp(minTree->children_);
    this->meld(tmp);
    this->shrink();
    size_ = was - 1;
    updateMin();
    return min;
}
