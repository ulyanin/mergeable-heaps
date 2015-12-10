#include "binomial_heap.h"
#include <functional>
#include <algorithm>
#include <iostream>
#include <climits>

const int BinomialHeap::KEY_INFINITY = INT_MAX;

inline size_t BinomialHeap::Node::childrenSize(BinomialHeap::Node * T)
{
    if (T == nullptr)
        return 0;
    return T->children_.size();
}

inline int BinomialHeap::Node::getKey(BNodePtr T)
{
    if (T == nullptr)
        return BinomialHeap::KEY_INFINITY;
    return T->key_;
}


BNodePtr BinomialHeap::Node::mergeTree(BNodePtr &A, BNodePtr &B)
{
    if (A == nullptr)
        return B;
    if (B == nullptr)
        return A;
    if (A->key_ > B->key_) {
        std::swap(A, B);
    }
    A->children_.push_back(B);
    B = nullptr;
    return A;
}

void BinomialHeap::Node::print(BNodePtr T, int d=0)
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
 * this function merge Trees A, B and addtitional like bits, save result in A and returns it
 *   0 + 0 = 0 and additional is 0
 *   0 + 1 = 1 + 0 = 1 and additional is 0
 *   1 + 1 = 0 and additional is 1
 * and then do not forget about additional
 */
void BinomialHeap::Node::mergeSaveDegree(BNodePtr &A, BNodePtr &B, BNodePtr &additional, size_t deg)
{
    A = mergeTree(A, B);
    B = nullptr;
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
        std::swap(A, additional);
    } else {
        /*
         * So A != 0 and there was not overflow
         * we just merge others
         */
        A = mergeTree(A, additional);
        additional = nullptr;
        if (childrenSize(A) != deg) {
            /*
             * if overflow, push this to additinal
             */
            std::swap(A, additional);
        }
    }
}

BinomialHeap::Node::Node(int key)
    : key_(key)
{}

BinomialHeap::Node::Node(const Node &other)
    : key_(other.key_)
{
    children_.assign(other.children_.size(), nullptr);
    for (size_t deg = 0; deg < other.children_.size(); ++deg)
    {
        children_[deg] = BinomialHeap::makeNewNode(other.children_[deg]);
    }
}

BinomialHeap::Node::~Node()
{
    for (auto &ptr : children_) {
        if (ptr)
            delete ptr;
    }
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

BNodePtr BinomialHeap::makeNewNode(Node * other)
{
    if (!other)
        return other;
    return new Node(*other);
}

void BinomialHeap::meld(IMergeableHeap<int> &otherBase)
{
    BinomialHeap & other = dynamic_cast<BinomialHeap &>(otherBase);
    BNodePtr overflow(nullptr);
    trees_.resize(std::max(this->maxDegree(), other.maxDegree()), nullptr);
    other.trees_.resize(std::max(this->maxDegree(), other.maxDegree()), nullptr);
    for (size_t deg = 0; deg < other.maxDegree(); ++deg) {
        Node::mergeSaveDegree(trees_[deg], other.trees_[deg], overflow, deg);
    }
    size_ += other.size_;
    other.clear();
    if (overflow != nullptr) {
        trees_.push_back(overflow);
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
    trees_.push_back(new BinomialHeap::Node(key));
}

BinomialHeap::BinomialHeap(const BinomialHeap &other)
    : size_(other.size_)
    , min_(other.min_)
{
    trees_.assign(other.maxDegree(), nullptr);
    for (size_t deg = 0; deg < other.maxDegree(); ++deg)
    {
        trees_[deg] = makeNewNode(other.trees_[deg]);
    }
}

BinomialHeap::BinomialHeap(BNodePtr T)
    : size_(Node::childrenSize(T))
    , min_(BinomialHeap::KEY_INFINITY)
{
    if (T) {
        trees_.push_back(T);
        min_ = T->key_;
    }
}

BinomialHeap::BinomialHeap(std::vector<Node *> &otherTrees)
    : size_((1 << otherTrees.size()) - 1)
{
    trees_.swap(otherTrees);
    updateMin();
}

void BinomialHeap::updateMin()
{
    min_ = BinomialHeap::KEY_INFINITY;
    for (auto ptr : trees_) {
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
    for (auto &ptr: trees_) {
        if (ptr) {
            delete ptr;
        }
    }
    trees_.clear();
    size_ = 0;
    min_ = BinomialHeap::KEY_INFINITY;
}


BinomialHeap::~BinomialHeap()
{
    for (auto &ptr: trees_) {
        if (ptr) {
            delete ptr;
        }
    }
}

int BinomialHeap::getMin() const
{
    if (this->trees_.back() == nullptr)
        throw std::runtime_error("last element of heap is empty");
    int min = BinomialHeap::KEY_INFINITY;
    for (auto ptr : trees_) {
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
    Node * minTree = trees_.back();
    int min = Node::getKey(minTree);
    for (auto &ptr: trees_) {
        if (ptr != nullptr && Node::getKey(ptr) < min) {
            minTree = ptr;
            min = Node::getKey(ptr);
        }
    }
    size_t was = size_;
    trees_[Node::childrenSize(minTree)] = nullptr;
    BinomialHeap tmp(minTree->children_);
    this->meld(tmp);
    this->shrink();
    delete minTree;
    size_ = was - 1;
    updateMin();
    return min;
}
