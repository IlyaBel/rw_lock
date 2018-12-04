#include "rw_lock.h"

RWMutex::RWMutex() : read_work(0), write_queue(0), write_work(false) {}

void RWMutex::read_lock(){
    std::unique_lock<std::mutex> lck(rw_mutex);
    read_cv.wait(lck, [this]{return write_queue == 0 && !write_work;});
    ++read_work;
}

void RWMutex::read_unlock(){
    {
        std::lock_guard<std::mutex> lck(rw_mutex);
        --read_work;
    }
    write_cv.notify_one();
}

void RWMutex::write_lock(){
    std::unique_lock<std::mutex> lck(rw_mutex);
    ++write_queue;
    write_cv.wait(lck, [this]{return !write_work && read_work == 0;});
    --write_queue;
    write_work = true;
}

void RWMutex::write_unlock(){
    {
        std::lock_guard<std::mutex> lck(rw_mutex);
        write_work = false;
    }
    write_cv.notify_one();
    read_cv.notify_all();
}

ReadLockGuard::ReadLockGuard(RWMutex &rw_mutex) : rw_mutex_(rw_mutex){
    rw_mutex_.read_lock();
}

ReadLockGuard::~ReadLockGuard(){
    rw_mutex_.read_unlock();
}

WriteLockGuard::WriteLockGuard(RWMutex &rw_mutex) : rw_mutex_(rw_mutex){
    rw_mutex_.write_lock();
}

WriteLockGuard::~WriteLockGuard(){
    rw_mutex_.write_unlock();
}
