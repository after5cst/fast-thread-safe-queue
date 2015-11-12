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
}
#endif // FTSQ_MUTEX_DOT_H
