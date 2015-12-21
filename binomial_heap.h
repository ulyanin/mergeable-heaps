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
    typedef std::unique_ptr<BinomialHeap::Node> NodePtr;
    void updateMin();
    BinomialHeap(std::vector<NodePtr> &);
    BinomialHeap(NodePtr &);
    static NodePtr makeNewNode(const NodePtr &);
    void shrink();
    size_t maxDegree() const;
    std::vector <NodePtr> trees_;
    size_t size_;
    int min_;
};


class BinomialHeap::Node
{
    friend BinomialHeap;
public:
    Node(int=0);
    ~Node();
    static size_t childrenSize(const NodePtr &);
    static NodePtr mergeTree(NodePtr &, NodePtr &);
    static void mergeSaveDegree(NodePtr &, NodePtr &, NodePtr &, size_t);
    static int getKey(const NodePtr &);
    static void print(const NodePtr &, int);
private:
    Node(const Node &);
    int key_;
    std::vector<NodePtr> children_;
};


#endif
