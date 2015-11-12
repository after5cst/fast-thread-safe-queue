#include <deque>
#include <iostream>
#include <thread>

#include "ftsq/measure.h"
#include "ftsq/fast_queue.h"

const auto count = 100000000;

int total;

void control()
{
    std::deque<int> d;
    for(auto i = 0; i < count; ++i)
    {
        d.push_back(i);
    }
    while(!d.empty())
    {
        total += d.front();
        d.pop_front();
    }
}

int multi_consumer(std::deque<int>* q, std::mutex *mtx, std::atomic_flag* loaded)
{
    ftsq::fast_queue<int>::queue_type items;
    while(loaded->test_and_set())
    {
        std::lock_guard<std::mutex> guard(*mtx);
        if (!q->empty())
        {
            total += q->front();
            q->pop_front();
        }
    }
    std::lock_guard<std::mutex> guard(*mtx);
    if (!q->empty())
    {
        total += q->front();
        q->pop_front();
    }
    return total;
}

void multi_queue_test()
{
    std::deque<int> d;
    std::mutex mtx;
    std::atomic_flag loaded;
    loaded.test_and_set();

    std::thread reader(multi_consumer,&d, &mtx, &loaded);

    for(auto i = 0; i < count; ++i)
    {
        std::lock_guard<std::mutex> guard(mtx);
        d.push_back(i);
    }
    loaded.clear();
    reader.join();
}

int fast_consumer(ftsq::fast_queue<int>* q, std::atomic_flag* loaded)
{
    ftsq::fast_queue<int>::queue_type items;
    while(loaded->test_and_set())
    {
        q->pop(items);
        for (auto& item : items)
        {
            total += item;
        }
    }
    q->pop(items);
    for (auto& item : items)
    {
        total += item;
    }
    return total;
}

void fast_queue_test()
{
    ftsq::fast_queue<int> d;
    std::atomic_flag loaded;
    loaded.test_and_set();

    std::thread reader(fast_consumer,&d,&loaded);

    for(auto i = 0; i < count; ++i)
    {
        d.push(i);
    }
    loaded.clear();
    reader.join();
}

int main()
{
    total = 0;

    auto control_duration =  ftsq::measure<>::execution(control);
    std::cout << "Control " << control_duration << " ms" << std::endl;

    auto multi_test_duration =  ftsq::measure<>::execution(multi_queue_test);
    std::cout << "Multi Test " << multi_test_duration << " ms" << std::endl;

    auto fast_test_duration =  ftsq::measure<>::execution(fast_queue_test);
    std::cout << "Fast Test " << fast_test_duration << " ms" << std::endl;

    std::cout << "Total (ignore me)" << total << std::endl;

}
