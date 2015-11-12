#ifndef FTSQ_MUTEX_H
#define FTSQ_MUTEX_H

#include "ftsq/mutex.h"
#  include <vector>
#  include <deque>

namespace ftsq
{
    template <typename T, typename mutex_type=ftsq::mutex>
    class queue_pop_one
    {
    public:
        typedef std::deque<T> queue_type;
        typedef typename queue_type::size_type size_type;

        size_type push(T item)
        {
            std::lock_guard<mutex_type> guard(m_mutex);
            m_queue.push_back(std::move(item));
            return m_queue.size();
        }

        bool pop(T& item)
        {
            std::lock_guard<mutex_type> guard(m_mutex);
            if(m_queue.empty())
            {
                return false;
            }
            item = std::move(m_queue.front());
            m_queue.pop_front();
            return true;
        }

        queue_pop_one() {}
        // disable object copy
        queue_pop_one(const queue_pop_one&) = delete;
        void operator=(const queue_pop_one&) = delete;
    private:
        mutex_type m_mutex;
        queue_type m_queue;
    }; //class queue_pop_one

    template <typename T, typename mutex_type=ftsq::mutex>
    class queue_pop_all
    {
    public:
        typedef std::vector<T> queue_type;
        typedef typename queue_type::size_type size_type;

        size_type push(T item)
        {
            std::lock_guard<mutex_type> guard(m_mutex);
            m_queue.push_back(std::move(item));
            return m_queue.size();
        }

        queue_type pop_all()
        {
            std::lock_guard<mutex_type> guard(m_mutex);
            return std::move(m_queue);
        }

        queue_pop_all() {}
        // disable object copy
        queue_pop_all(const queue_pop_all&) = delete;
        void operator=(const queue_pop_all&) = delete;
    private:
        mutex_type m_mutex;
        queue_type m_queue;
    }; //class queue_pop_all
}
#endif // FTSQ_MUTEX_H
