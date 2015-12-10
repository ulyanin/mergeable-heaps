#include <iostream>
#include <gtest/gtest.h>
#include "imergeable_heap.h"
#include "binomial_heap.h"
#include "simple_heap.h"
#include "skew_heap.h"
#include "leftist_heap.h"
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>

const int SMALL_TEST = 10;
const int MEDIUM_TEST = 1e4;
const int LARGE_TEST = 1e5;
const int EXTRA_LARGE_TEST = 1e7;
const int MAX_TEST = 1e8;

class HeapsTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {}

    virtual void TearDown()
    {
        for (auto &ptr : binomialHeaps) {
            delete ptr;
        }
        for (auto &ptr : simpleHeaps) {
            delete ptr;
        }
        for (auto &ptr : skewHeaps) {
            delete ptr;
        }
        for (auto &ptr : leftistHeaps) {
            delete ptr;
        }
        binomialHeaps.clear();
        simpleHeaps.clear();
        skewHeaps.clear();
        leftistHeaps.clear();
    }

    void AddHeap(int key)
    {
        binomialHeaps.push_back(new BinomialHeap(key));
        skewHeaps.push_back(new SkewHeap(key));
        simpleHeaps.push_back(new SimpleHeap(key));
        leftistHeaps.push_back(new LeftistHeap(key));
    }

    void Insert(int index, int key)
    {
        binomialHeaps[index]->insert(key);
        simpleHeaps[index]->insert(key);
        skewHeaps[index]->insert(key);
        leftistHeaps[index]->insert(key);
    }

    int ExtractMin(int index)
    {
        int m0 = simpleHeaps[index]->extractMin();
        int m1 = binomialHeaps[index]->extractMin();
        int m2 = skewHeaps[index]->extractMin();
        int m3 = leftistHeaps[index]->extractMin();
        EXPECT_EQ(m0, m1) << "in binomial heap #" << index << "; bad minimum extracted";
        EXPECT_EQ(m0, m2) << "in skew heap #" << index << "; bad minimum extracted";
        EXPECT_EQ(m0, m3) << "in leftist heap #" << index << "; bad minimum extracted";
        return m0;
    }

    bool Empty(int index) const
    {
        return simpleHeaps[index]->empty();
    }

    bool Empty() const
    {
        return simpleHeaps.empty();
    }

    size_t Size() const
    {
        return simpleHeaps.size();
    }

    void TestSize(int index) const
    {
        size_t s0 = simpleHeaps[index]->size();
        size_t s1 = binomialHeaps[index]->size();
        size_t s2 = skewHeaps[index]->size();
        size_t s3 = leftistHeaps[index]->size();
        EXPECT_EQ(s0, s1) << "in binomial heap #" << index << "; bad size";
        EXPECT_EQ(s0, s2) << "in skew heap #" << index << "; bad size";
        EXPECT_EQ(s0, s3) << "in leftist heap #" << index << "; bad size";
    }

    void TestEmpty(int index) const
    {
        bool e0 = simpleHeaps[index]->empty();
        bool e1 = binomialHeaps[index]->empty();
        bool e2 = skewHeaps[index]->empty();
        bool e3 = leftistHeaps[index]->empty();
        EXPECT_EQ(e0, e1) << "in binomial heap #" << index
            << "; error while getting empty;";
        EXPECT_EQ(e0, e2) << "in skew heap #" << index
            << "; error while getting empty;";
        EXPECT_EQ(e0, e3) << "in leftist heap #" << index
            << "; error while getting empty;";
    }

    void Meld(int index1, int index2)
    {
        binomialHeaps[index1]->meld(*binomialHeaps[index2]);
        std::swap(binomialHeaps[index2], binomialHeaps.back());

        simpleHeaps[index1]->meld(*simpleHeaps[index2]);
        std::swap(simpleHeaps[index2], simpleHeaps.back());

        leftistHeaps[index1]->meld(*leftistHeaps[index2]);
        std::swap(leftistHeaps[index2], leftistHeaps.back());

        skewHeaps[index1]->meld(*skewHeaps[index2]);
        std::swap(skewHeaps[index2], skewHeaps.back());

        /*skewHeaps[index1]->print();
        std::cout << "______________\n";
        leftistHeaps[index1]->print();
        std::cout << "leftist______________\n";
        std::cout << "______________\n";*/
    }

    void TestAdd()
    {
        int key = random(engine) % 100;
        AddHeap(key);
    }

    void TestInsert()
    {
        int index = random(engine) % Size();
        int key = random(engine) % 100;
        Insert(index, key);
        TestEmpty(index);
        TestSize(index);
    }

    void TestExtractMin()
    {
        int index = random(engine) % Size();
        if (!Empty(index))
            ExtractMin(index);
        TestEmpty(index);
        TestSize(index);
    }

    void TestMeld()
    {
        int index1 = random(engine) % Size();
        int index2 = random(engine) % (Size() - 1);
        if (index2 == index1)
            index2 = Size() - 1;
        Meld(index1, index2);
        TestEmpty(index1);
        TestSize(index1);
    }

    void MakeAllEmpty()
    {
        for (size_t i = 0; i < Size(); ++i) {
            while (!Empty(i)) {
                ExtractMin(i);
            }
            TestSize(i);
            TestEmpty(i);
        }
    }

    void TestCorrect(int toTest)
    {
        std::cout << "starting testing heaps correctly with "
            << toTest << " operations" << std::endl;
        for (int i = 0; i < toTest; ++i)
        {
            //std::cout << i << std::endl;
            int way = random(engine) % 4;
            //std::cout << way << std::endl;
            if (way == 0) {
                TestAdd();
            } else if (!Empty() && way == 1) {
                TestInsert();
            } else if (!Empty() && way == 2) {
                TestExtractMin();
            } else if (Size() > 1 && way == 3) {
               TestMeld();
            } else {
                --i;
            }
        }
        MakeAllEmpty();
    }

public:
    std::vector<BinomialHeap *> binomialHeaps;
    std::vector<SkewHeap *> skewHeaps;
    std::vector<LeftistHeap *> leftistHeaps;
    std::vector<SimpleHeap *> simpleHeaps;
    std::uniform_int_distribution<int> random;
    std::default_random_engine engine;
};


class HeapPerfomanceTest : public ::testing::Test
{
protected:
    typedef IMergeableHeap<int> * HeapPtr;
    virtual void SetUp()
    {}

    virtual void TearDown()
    {
        for (auto &ptr : heaps) {
            delete ptr;
        }
        heaps.clear();
    }

    void Insert(int index, int key)
    {
        heaps[index]->insert(key);
    }

    int ExtractMin(int index)
    {
        return heaps[index]->extractMin();
    }

    bool Empty(int index) const
    {
        return heaps[index]->empty();
    }

    bool Empty() const
    {
        return heaps.empty();
    }

    size_t Size() const
    {
        return heaps.size();
    }

    void Meld(int index1, int index2)
    {
        heaps[index1]->meld(*heaps[index2]);
        std::swap(heaps[index2], heaps.back());

        /*skewHeaps[index1]->print();
        std::cout << "______________\n";
        leftistHeaps[index1]->print();
        std::cout << "leftist______________\n";
        std::cout << "______________\n";*/
    }


    void TestInsert()
    {
        int index = random(engine) % Size();
        int key = random(engine) % 100;
        Insert(index, key);
    }

    void TestExtractMin()
    {
        int index = random(engine) % Size();
        if (!Empty(index))
            ExtractMin(index);
    }

    void TestMeld()
    {
        int index1 = random(engine) % Size();
        int index2 = random(engine) % (Size() - 1);
        if (index2 == index1)
            index2 = Size() - 1;
        Meld(index1, index2);
    }

    void MakeAllEmpty()
    {
        for (size_t i = 0; i < Size(); ++i) {
            while (!Empty(i)) {
                ExtractMin(i);
            }
        }
    }
    virtual void TestTimeHeap(std::vector<HeapPtr> &heapsEmpty, int operationsNumber)
    {
        heaps.swap(heapsEmpty);
        std::cout << "starting testing current heap woriking time with "
            << operationsNumber << " operations" << std::endl;
        for (int i = 0; i < operationsNumber; ++i)
        {
            //std::cout << i << std::endl;
            int way = random(engine) % 3;
            //std::cout << way << std::endl;
            if (!Empty() && way == 0) {
                TestInsert();
            } else if (!Empty() && way == 1) {
                TestExtractMin();
            } else if (Size() > 1 && way == 2) {
               TestMeld();
            } else {
                --i;
            }
        }
        MakeAllEmpty();
    }
public:
    std::vector<HeapPtr> heaps;
    std::uniform_int_distribution<int> random;
    std::default_random_engine engine;
};


class HeapCompareWithSortTest : public ::testing::Test
{
protected:
    typedef IMergeableHeap<int> * HeapPtr;
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    double TestStdSort(std::vector <int> &toSort)
    {
        auto start(std::chrono::steady_clock::now());
        std::sort(toSort.begin(), toSort.end());
        auto end(std::chrono::steady_clock::now());
        return std::chrono::duration_cast<std::chrono::duration<double> > (end - start).count();
    }

    double TestHeapSort(HeapPtr heap, std::vector <int> toSort)
    {
        auto start(std::chrono::steady_clock::now());
        for (int elem : toSort) {
            heap->insert(elem);
        }
        for (auto &elem : toSort) {
            elem = heap->extractMin();
        }
        auto end(std::chrono::steady_clock::now());
        return std::chrono::duration_cast<std::chrono::duration<double> > (end - start).count();
    }

    void TestWith(HeapPtr heap, size_t size)
    {
        std::vector <int> array(size);
        generate(array.begin(), array.end(), [this] () { return random(engine); } );
        std::vector <int> toHeap(array);
        std::cout << "start sorting" << std::endl;
        double d1 = TestStdSort(array);
        std::cout << "sorting time with std::sort is " << d1 << " seconds" << std::endl;
        std::cout << "start sorting" << std::endl;
        double d2 = TestHeapSort(heap, toHeap);
        std::cout << "sorting time with heap is " << d2 << " seconds" << std::endl;
        std::cout << "relative is " << d2 / d1 << std::endl;
    }


public:
    std::uniform_int_distribution<int> random;
    std::default_random_engine engine;
};




TEST_F(HeapsTest, TestCorrectSmall)
{
    TestCorrect(SMALL_TEST);
}

TEST_F(HeapsTest, TestCorrectMedium)
{
    TestCorrect(MEDIUM_TEST);
}

TEST_F(HeapPerfomanceTest, TestSkewLarge)
{
    int testSize = EXTRA_LARGE_TEST;
    int n = (int)log((float)testSize);
    std::vector <IMergeableHeap<int> *> heaps(n, nullptr);
    generate(heaps.begin(), heaps.end(), [] () { return new SkewHeap(); });
    TestTimeHeap(heaps, testSize);
}

TEST_F(HeapPerfomanceTest, TestBinomialLarge)
{
    int testSize = EXTRA_LARGE_TEST;
    int n = (int)log((float)testSize);
    std::vector <IMergeableHeap<int> *> heaps(n, nullptr);
    generate(heaps.begin(), heaps.end(), [] () { return new BinomialHeap(); });
    TestTimeHeap(heaps, testSize);
}

TEST_F(HeapPerfomanceTest, TestLeftistHeap)
{
    int testSize = EXTRA_LARGE_TEST;
    int n = (int)log((float)testSize);
    std::vector <IMergeableHeap<int> *> heaps(n, nullptr);
    generate(heaps.begin(), heaps.end(), [] () { return new LeftistHeap(); });
    TestTimeHeap(heaps, testSize);
}

TEST_F(HeapPerfomanceTest, TestSimpleHeap)
{
    int testSize = 1e4;
    int n = (int)log((float)testSize);
    std::vector <IMergeableHeap<int> *> heaps(n, nullptr);
    generate(heaps.begin(), heaps.end(), [] () { return new SimpleHeap(); });
    TestTimeHeap(heaps, testSize);
}

TEST_F(HeapCompareWithSortTest, TestSimpleHeap)
{
    int testSize = LARGE_TEST;
    HeapPtr heap = new SimpleHeap();
    TestWith(heap, testSize);
    delete heap;
}

TEST_F(HeapCompareWithSortTest, TestBinomialHeap)
{
    int testSize = LARGE_TEST;
    HeapPtr heap = new BinomialHeap();
    TestWith(heap, testSize);
    delete heap;
}

TEST_F(HeapCompareWithSortTest, TestSkewHeap)
{
    int testSize = LARGE_TEST;
    HeapPtr heap = new SkewHeap();
    TestWith(heap, testSize);
    delete heap;
}

TEST_F(HeapCompareWithSortTest, TestLeftistHeap)
{
    int testSize = LARGE_TEST;
    HeapPtr heap = new LeftistHeap();
    TestWith(heap, testSize);
    delete heap;
}

int main(int argc, char **argv)
{
    std::cerr.setf(std::cerr.fixed);
    std::cerr.precision(9);

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
