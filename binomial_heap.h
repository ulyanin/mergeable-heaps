#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H
#include <vector>
#include <functional>
#include <memory>
#include "imergeable_heap.h"

class BinomialHeap : public IMergeableHeap<int, std::less<int> >
{
public:
    class Node;
    virtual ~BinomialHeap() {}
    BinomialHeap(int);
    BinomialHeap(Node *);
    virtual BinomialHeap meld(const BinomialHeap &);
    virtual void insert(const int &);
    virtual int extractMin();
    virtual int getMin() const;
    virtual size_t size() const;
    virtual bool empty() const;
    static int KEY_INFINITY;
private:
    size_t maxDegree() const;
    std::vector <Node * > trees_;
};

typedef BinomialHeap::Node * BNodePtr;
typedef const BinomialHeap::Node * CBNodePtr;

int BinomialHeap::KEY_INFINITY = 2e9;

class BinomialHeap::Node
{
    friend BinomialHeap;
public:
    Node(int=0);
    ~Node();
    friend size_t degree(Node *);
    friend Node * mergeTree(Node * &, Node * &);
    friend Node * mergeSaveDegree(Node * &, Node * &, Node * &);
    friend int getKey(Node *);
private:
    int key_;
    std::vector<Node *> children_;
};


#endif
