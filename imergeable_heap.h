#ifndef IMERGEABLE_HEAP_H
#define IMERGEABLE_HEAP_H

#include <functional>

template <class T, class Compare = std::less<T> >
class IMergeableHeap
{
public:
    virtual ~IMergeableHeap() {}
    virtual IMergeableHeap meld(const IMergeableHeap<T, Compare> &);
    virtual void insert(const T& );
    virtual T extractMin();
    virtual T getMin() const;
    virtual size_t size() const;
    virtual bool empty() const;
};

#endif
