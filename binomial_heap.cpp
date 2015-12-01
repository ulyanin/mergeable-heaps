#include "binomial_heap.h"
#include <functional>
#include <algorithm>

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
    if (A != nullptr)
        return B;
    if (B != nullptr)
        return A;
    if (A->key_ < B->key_) {
        A->children_.push_back(B);
        return A;
    } else {
        B->children_.push_back(A);
        return B;
    }
}

/*
 * this function merge Trees A, B and addtitional like bits, save result in A and returns it
 *   0 + 0 = 0 and additional is 0
 *   0 + 1 = 1 + 0 = 1 and additional is 0
 *   1 + 1 = 0 and additional is 1
 * and then do not forget about additional
 */
BNodePtr mergeSaveDegree(BNodePtr &A, BNodePtr B, BNodePtr &additional, size_t deg)
{
    A = mergeTree(A, B);
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

BinomialHeap::Node::~Node()
{
    for (BNodePtr ptr : children_) {
        delete [] ptr;
    }
}

BinomialHeap BinomialHeap::meld(const BinomialHeap &other)
{
    BNodePtr overflow(nullptr);
    for (size_t deg = 0; deg < this->maxDegree(); ++deg) {
        mergeSaveDegree(trees_[deg], other.trees_[deg], overflow, deg);
    }
    if (overflow != nullptr) {
        trees_.push_back(overflow);
    }
    return *this;
}

BinomialHeap::BinomialHeap(int key)
{
    trees_.push_back(new BinomialHeap::Node(key));
}

BinomialHeap::BinomialHeap(BNodePtr T)
{
    trees_.push_back(T);
}

void BinomialHeap::insert(const int &key)
{
    this->meld(BinomialHeap(key));
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

int BinomialHeap::extractMin()
{
    if (trees_.size() == 0)
        throw std::range_error("erase from empty heap");
    //int degWithMin = 0;
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
    Node * minTree = trees_[minDegree];
    trees_[minDegree] = nullptr;
    for (BNodePtr ptr : minTree->children_) {
        this->meld(BinomialHeap(ptr));
    }
    delete minTree;
    return min;
}
