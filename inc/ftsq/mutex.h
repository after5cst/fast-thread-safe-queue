#ifndef FTSQ_MUTEX_DOT_H
#define FTSQ_MUTEX_DOT_H

#include <atomic>
#include <mutex>

namespace ftsq
{
    class mutex
    {
    public:
        mutex() : m_count(0), m_mutex() { m_mutex.lock(); }
        mutex(const mutex&) = delete;
        void operator=(const mutex&) = delete;

        void lock() {
            if(0 < m_count++) {
                m_mutex.lock();
            }
        }

        void unlock() {
            if( 0 < --m_count) {
                m_mutex.unlock();
            }
        }

    private:
        std::atomic_int m_count;
        std::mutex m_mutex;
    }; //class mutex

    class spinlock
    {
    public:
        spinlock() { m_flag.clear(); }
        spinlock(const spinlock&) = delete;
        void operator=(const spinlock&) = delete;

        void lock() {
            while(m_flag.test_and_set(std::memory_order_acquire));
        }

        void unlock() {
            m_flag.clear(std::memory_order_release);
        }

    private:
        std::atomic_flag m_flag;
    }; //class mutex
}
#endif // FTSQ_MUTEX_DOT_H
