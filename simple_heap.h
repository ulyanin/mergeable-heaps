#ifndef SIMPLE_HEAP_H
#define SIMPLE_HEAP_H

#include "imergeable_heap.h"
#include <set>
#include <iostream>

class SimpleHeap : public IMergeableHeap<int>
{
public:
    ~SimpleHeap() {}
    SimpleHeap(){}
    SimpleHeap(const int &k)
    {
        data_.insert(k);
    }
    void meld(SimpleHeap &other)
    {
        while (!other.empty()) {
            this->insert(other.extractMin());
        }
    }
    void insert(const int &k)
    {
        data_.insert(k);
    }
    int extractMin()
    {
        int min = this->getMin();
        data_.erase(data_.begin());
        return min;
    }
    int getMin() const
    {
        return *data_.begin();
    }
    size_t size() const
    {
        return data_.size();
    }
    bool empty() const
    {
        return data_.empty();
    }
    void clear()
    {
        data_.clear();
    }

private:
    std::multiset <int> data_;
};

#endif
