#ifndef FTSQ_MUTEX_H
#define FTSQ_MUTEX_H

#include "ftsq/mutex.h"
#ifdef FTSQ_POP_ALL
#  include <vector>
#else
#  include <deque>
#endif // FTSQ_POP_ALL

namespace ftsq
{
    template <typename T, typename mutex_type=ftsq::mutex>
    class fast_queue
    {
    public:
#ifdef FTSQ_POP_ALL
        typedef std::vector<T> queue_type;
#else
        typedef std::deque<T> queue_type;
#endif
        typedef typename queue_type::size_type size_type;

        size_type push(T item)
        {
            std::lock_guard<mutex_type> guard(m_mutex);
            m_queue.push_back(std::move(item));
            return m_queue.size();
        }

#ifdef FTSQ_POP_ALL
        queue_type pop_all()
        {
            std::lock_guard<mutex_type> guard(m_mutex);
            return std::move(m_queue);
        }
#else
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
#endif

        fast_queue() {}
        // disable object copy
        fast_queue(const fast_queue&) = delete;
        void operator=(const fast_queue&) = delete;
    private:
        mutex_type m_mutex;
        queue_type m_queue;
    }; //class mutex
}
#endif // FTSQ_MUTEX_H
