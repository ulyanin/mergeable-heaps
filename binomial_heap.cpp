#include "binomial_heap.h"
#include <functional>
#include <algorithm>
#include <iostream>

int BinomialHeap::KEY_INFINITY = 2e9;

inline size_t degree(BinomialHeap::Node * T)
{
    if (T == nullptr)
        return -1;
    return T->children_.size();
}

inline int getKey(BNodePtr T)
{
    if (T == nullptr)
        return BinomialHeap::KEY_INFINITY;
    return T->key_;
}


BNodePtr mergeTree(BNodePtr &A, BNodePtr &B)
{
    if (A == nullptr)
        return B;
    if (B == nullptr)
        return A;
    if (A->key_ < B->key_) {
        A->children_.push_back(B);
        B = nullptr;
        return A;
    } else {
        B->children_.push_back(A);
        A = nullptr;
        return B;
    }
}

void print(BNodePtr T, int d=0)
{
    if (!T)
        return;
    for (int i = 0; i < d; ++i)
        std::cout << "\t";
    std::cout << T->key_ << std::endl;
    for (size_t deg = 0; deg < degree(T); ++deg) {
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
BNodePtr mergeSaveDegree(BNodePtr &A, BNodePtr &B, BNodePtr &additional, size_t deg)
{
    A = mergeTree(A, B);
    B = nullptr;
    if (degree(A) != deg)
    {
        /*
         * if merging A and B returns "0" (with overflow or not)
         */
        std::swap(A, additional);
        return A;
    }
    A = mergeTree(A, additional);
    additional = nullptr;
    if (degree(A) > deg) {
        std::swap(A, additional);
    }
    return A;
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
        children_[deg] = makeNewNode(other.children_[deg]);
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
        ::print(trees_[deg]);
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

BNodePtr BinomialHeap::Node::makeNewNode(Node * other)
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
        mergeSaveDegree(trees_[deg], other.trees_[deg], overflow, deg);
    }
    size_ += other.size_;
    other.clear();
    if (overflow != nullptr) {
        trees_.push_back(overflow);
    }
    //this->print();
}

BinomialHeap::BinomialHeap()
    : size_(0)
{}

BinomialHeap::BinomialHeap(const int &key)
    : size_(1)
{
    trees_.push_back(new BinomialHeap::Node(key));
}

BinomialHeap::BinomialHeap(const BinomialHeap &other)
    : size_(other.size_)
{
    trees_.assign(other.maxDegree(), nullptr);
    for (size_t deg = 0; deg < other.maxDegree(); ++deg)
    {
        trees_[deg] = makeNewNode(other.trees_[deg]);
    }
}

BinomialHeap::BinomialHeap(BNodePtr T)
    : size_(degree(T))
{
    trees_.push_back(T);
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
}


BinomialHeap::~BinomialHeap()
{
    this->clear();
}

int BinomialHeap::getMin() const
{
    if (this->trees_.back() == nullptr)
        throw std::runtime_error("last element of heap is empty");
    int min = BinomialHeap::KEY_INFINITY;
    for (auto ptr : trees_) {
        if (ptr != nullptr) {
            min = std::min(min, getKey(ptr));
        }
    }
    return min;
}

size_t BinomialHeap::size() const
{
    /*size_t sum = 0;
    for (size_t deg = 0; deg < trees_.size(); ++deg) {
        if (trees_[deg] != nullptr)
            sum += 1 << deg;
    }
    return sum;*/
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
    int minDegree = 0;
    int min = getKey(trees_[0]);
    for (size_t deg = 0; deg < trees_.size(); ++deg) {
        if (trees_[deg] != nullptr) {
            if (getKey(trees_[deg]) < min) {
                minDegree = deg;
                min = getKey(trees_[deg]);
            }
        }
    }
    size_t was = size_;
    Node * minTree = trees_[minDegree];
    trees_[minDegree] = nullptr;
    for (BNodePtr &ptr : minTree->children_) {
        BinomialHeap tmp(ptr);
        this->meld(tmp);
        ptr = nullptr;
    }
    this->shrink();
    delete minTree;
    size_ = was - 1;
    return min;
}
