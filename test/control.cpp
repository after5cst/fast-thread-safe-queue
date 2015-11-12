#include <assert.h>
#include <deque>
#include <iostream>
#include <thread>

#define FTSQ_POP_ALL
#include "ftsq/measure.h"
#include "ftsq/fast_queue.h"

const auto count = 100000000;

template <typename T> void consumer(T *q, std::atomic_flag *loading)
{
    auto total = 0;
#ifdef FTSQ_POP_ALL
    typename T::queue_type items;
    while (loading->test_and_set())
    {
        items = q->pop_all();
        for (auto &item : items)
        {
            total += item;
        }
    }
    // Nothing new will show up now: just pick up what's left.
    items = q->pop_all();
    for (auto &item : items)
    {
        total += item;
    }
#else // FTSQ_POP_ALL not defined
    int item;
    while (loading->test_and_set())
    {
        while(q->pop(item))
        {
            total += item;
        }
    }
    // Nothing new will show up now: just pick up what's left.
    while(q->pop(item))
    {
        total += item;
    }
#endif // FTSQ_POP_ALL
    assert(total == count);
}

template <typename T> void producer()
{
    T d;
    std::atomic_flag loading;
    loading.test_and_set();

    std::thread reader(consumer<T>, &d, &loading);

    for (auto i = 0; i < count; ++i)
    {
        d.push(1);
    }
    loading.clear();
    reader.join();
}

int main()
{
    auto stdmutex_duration = ftsq::measure<>::execution(
                producer<ftsq::fast_queue<int,std::mutex> >);
    std::cout << "std::mutex " << stdmutex_duration << " ms" << std::endl;

    auto ftsqmutex_duration = ftsq::measure<>::execution(
                producer<ftsq::fast_queue<int,std::mutex> >);
    std::cout << "ftsq::mutex " << ftsqmutex_duration << " ms" << std::endl;
    return 0;
}
