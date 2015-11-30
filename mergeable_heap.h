#ifndef MERGEABLE_HEAP_H
#define MERGEABLE_HEAP_H

template <class T, class CompareFuntion>
class MergeableHeap
{
public:
    virtual MergeableHeap();
    virtual ~MergeableHeap() {}
    virtual MergeableHeap meld(MergeableHeap<T, CompareFunction> &,
                       MergeableHeap<T, CompareFunction> &);
    virtual void insert(const T& );
    virtual T extractMin();
    virtual const T& getMin() const;
    virtual size_t size() const;
    virtual bool empty() const;
};

#endif
