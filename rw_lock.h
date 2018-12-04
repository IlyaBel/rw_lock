#ifndef RW_LOCK_H
#define RW_LOCK_H

#include <mutex>
#include <condition_variable>

class RWMutex {
private:
    std::mutex rw_mutex;
    std::condition_variable read_cv;
    std::condition_variable write_cv;

    size_t read_work;
    size_t write_queue;
    //write_work: 1 - writer is writing now, 0 - is free
    bool write_work;

public:
    RWMutex();

    void read_lock();

    void read_unlock();

    void write_lock();

    void write_unlock();
};

class ReadLockGuard{
private:
    RWMutex& rw_mutex_;

public:
    ReadLockGuard(RWMutex& rw_mutex);

    ~ReadLockGuard();
};

class WriteLockGuard{
private:
    RWMutex& rw_mutex_;

public:
    WriteLockGuard(RWMutex& rw_mutex);

    ~WriteLockGuard();
};

#endif // RW_LOCK_H
