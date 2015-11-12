#ifndef FTSQ_MUTEX_H
#define FTSQ_MUTEX_H

#include "ftsq/mutex.h"
#include <deque>

namespace ftsq
{
    template <typename T> class fast_queue
    {
    public:
        typedef std::deque<T> queue_type;
        typedef typename queue_type::size_type size_type;

        size_type push(T item)
        {
            std::lock_guard<ftsq::mutex> guard(m_mutex);
            m_queue.push_back(std::move(item));
            return m_queue.size();
        }

        size_type pop( T& item )
        {
            std::lock_guard<ftsq::mutex> guard(m_mutex);
            auto result = m_queue.size();
            if (0 < result)
            {
                std::swap(item, m_queue.front());
                m_queue.pop_front();
            }
            return result;
        }

        void pop( queue_type& items )
        {
            std::lock_guard<ftsq::mutex> guard(m_mutex);
            items = std::move(m_queue);
        }

        fast_queue() {}
        // disable object copy
        fast_queue(const fast_queue&) = delete;
        void operator=(const fast_queue&) = delete;
    private:
        ftsq::mutex m_mutex;
        queue_type m_queue;
    }; //class mutex
}
#endif // FTSQ_MUTEX_H
