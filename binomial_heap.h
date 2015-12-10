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
    virtual void meld(IMergeableHeap<int> &);
    virtual void insert(const int &);
    virtual int extractMin();
    virtual int getMin() const;
    virtual size_t size() const;
    virtual bool empty() const;
    virtual void clear();
    static const int KEY_INFINITY;
    void print() const;
private:
    void updateMin();
    BinomialHeap(std::vector<Node *> &);
    BinomialHeap(Node *);
    static Node * makeNewNode(Node *);
    void shrink();
    size_t maxDegree() const;
    std::vector <Node * > trees_;
    size_t size_;
    int min_;
};

typedef BinomialHeap::Node * BNodePtr;
typedef const BinomialHeap::Node * CBNodePtr;


class BinomialHeap::Node
{
    friend BinomialHeap;
public:
    Node(int=0);
    ~Node();
    static size_t childrenSize(Node *);
    static Node * mergeTree(Node * &, Node * &);
    static void mergeSaveDegree(Node * &, Node * &, Node * &, size_t);
    static int getKey(Node *);
    static void print(Node *, int);
private:
    Node(const Node &);
    int key_;
    std::vector<Node *> children_;
};


#endif
