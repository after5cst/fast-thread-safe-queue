#include <assert.h>
#include <deque>
#include <iostream>
#include <thread>

#include "ftsq/measure.h"
#include "ftsq/fast_queue.h"

const auto count = 100000000;

template <typename T> void consume_one(T *q, std::atomic_flag *loading)
{
    auto total = 0;
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
    assert(total == count);
}

template <typename T> void producer_one()
{
    T d;
    std::atomic_flag loading;
    loading.test_and_set();

    std::thread reader(consume_one<T>, &d, &loading);

    for (auto i = 0; i < count; ++i)
    {
        d.push(1);
    }
    loading.clear();
    reader.join();
}



template <typename T> void consume_all(T *q, std::atomic_flag *loading)
{
    auto total = 0;
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
    assert(total == count);
}

template <typename T> void producer_all()
{
    T d;
    std::atomic_flag loading;
    loading.test_and_set();

    std::thread reader(consume_all<T>, &d, &loading);

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
                producer_one<ftsq::queue_pop_one<int,std::mutex> >);
    std::cout << "ONE-> std::mutex " << stdmutex_duration << " ms" << std::endl;

    auto ftsqmutex_duration = ftsq::measure<>::execution(
                producer_one<ftsq::queue_pop_one<int,std::mutex> >);
    std::cout << "ONE-> ftsq::mutex " << ftsqmutex_duration << " ms" << std::endl;

    stdmutex_duration = ftsq::measure<>::execution(
                producer_all<ftsq::queue_pop_all<int,std::mutex> >);
    std::cout << "ALL-> std::mutex " << stdmutex_duration << " ms" << std::endl;

    ftsqmutex_duration = ftsq::measure<>::execution(
                producer_all<ftsq::queue_pop_all<int,std::mutex> >);
    std::cout << "ALL-> ftsq::mutex " << ftsqmutex_duration << " ms" << std::endl;

    return 0;
}
