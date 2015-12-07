#ifndef IMERGEABLE_HEAP_H
#define IMERGEABLE_HEAP_H

#include <functional>

template <class T, class Compare = std::less<T> >
class IMergeableHeap
{
public:
    virtual ~IMergeableHeap() {}
    virtual void meld(IMergeableHeap<T, Compare> &) = 0;
    virtual void insert(const T& ) = 0;
    virtual T extractMin() = 0;
    virtual T getMin() const = 0;
    virtual size_t size() const = 0;
    virtual bool empty() const = 0;
    virtual void clear() = 0;
};

/*
template <class T, class CMP>
void IMergeableHeap<T, CMP>::meld(IMergeableHeap<T, CMP> &other)
{
    while (!other.empty()) {
        this->insert(other.extractMin());
    }
}
*/
/*
template <class T>
class BinaryTree
{
public:
    BinaryTree * left,
               * right;
    BinaryTree();
    virtual ~BinaryTree();
    BinaryTree(const T&);
    BinaryTree(const BinaryTree &);
    virtual void makeLeft(BinaryTree *);
    virtual void makeRight(BinaryTree *);
    virtual void recalc(){};
    virtual size_t size() const;
    T& getKey();
    BinaryTree*& getLeft();
    BinaryTree*& getRight();
    //template <class U>
protected:
    size_t size_;
    T key_;
};

template <class T>
size_t size(BinaryTree<T> *Tree)
{
    return Tree ? Tree->size() : 0;
}

template <class T>
BinaryTree<T>::BinaryTree()
    : left(nullptr)
    , right(nullptr)
    , size_(0)
    , key_()
{}

template <class T>
BinaryTree<T>::~BinaryTree<T>()
{
    delete left;
    delete right;
}

template <class T>
BinaryTree<T>::BinaryTree(const T& key)
    : left(nullptr)
    , right(nullptr)
    , size_(0)
    , key_(key)
{}

template <class T>
BinaryTree<T>::BinaryTree(const BinaryTree<T> &other)
    : size_(other.size_)
    , key_(other.key_)
{
    left = new BinaryTree(*other.left);
    right = new BinaryTree(*other.right);
}

template <class T>
void BinaryTree<T>::makeLeft(BinaryTree * ptr)
{
    left = ptr;
    size_ = ::size<T>(left) + ::size<T>(right) + 1;
    recalc();
}

template <class T>
void BinaryTree<T>::makeRight(BinaryTree * ptr)
{
    right = ptr;
    size_ = ::size<T>(left) + ::size<T>(right) + 1;
    recalc();
}

template <class T>
size_t BinaryTree<T>::size() const
{
    return size_;
}

template <class T>
T& BinaryTree<T>::getKey()
{
    return key_;
}

template <class T>
BinaryTree<T> *& BinaryTree<T>::getLeft()
{
    return left;
}

template <class T>
BinaryTree<T> *& BinaryTree<T>::getRight()
{
    return right;
}
*/
#endif
