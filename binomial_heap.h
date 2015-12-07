#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H
#include <vector>
#include <functional>
#include <memory>
#include "imergeable_heap.h"

class BinomialHeap : public IMergeableHeap<int>
{
public:
    class Node;
    virtual ~BinomialHeap();
    BinomialHeap();
    BinomialHeap(const BinomialHeap &);
    BinomialHeap(const int &);
    BinomialHeap(Node *);
    virtual void meld(IMergeableHeap<int> &);
    virtual void insert(const int &);
    virtual int extractMin();
    virtual int getMin() const;
    virtual size_t size() const;
    virtual bool empty() const;
    virtual void clear();
    static int KEY_INFINITY;
    Node * makeNewNode(Node *);
    void print() const;
private:
    void shrink();
    size_t maxDegree() const;
    std::vector <Node * > trees_;
    size_t size_;
};

typedef BinomialHeap::Node * BNodePtr;
typedef const BinomialHeap::Node * CBNodePtr;


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
    friend void print(Node *, int);
    Node * makeNewNode(Node *);
private:
    Node(const Node &);
    int key_;
    std::vector<Node *> children_;
};


#endif
