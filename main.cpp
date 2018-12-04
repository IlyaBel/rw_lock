#include "rw_lock.h"
#include <iostream>
#include <thread>
#include <vector>
#include <string>

using namespace std::chrono_literals;

void read(RWMutex& rw_mutex){
    //RAII read lock
    ReadLockGuard lck(rw_mutex);

    //read
    std::cout << "I am here reading!" << std::endl;
    std::this_thread::sleep_for(3s);
}

void write(RWMutex& rw_mutex){
    //RAII write lock
    WriteLockGuard lck(rw_mutex);

    //write
    std::cout << "I am here writing!" << std::endl;
    std::this_thread::sleep_for(0.3s);
}

int main()
{
    RWMutex rw_mutex;

    std::vector<std::thread> threads;

    std::string what_to_do;
    std::cout << "Please, enter what to do. Use spaces and Enter button. "
        "Type \"stop\" when you want to quit." << std::endl;

    do {
        std::cin >> what_to_do;

        if (what_to_do == "r")
            threads.push_back(std::thread(read, std::ref(rw_mutex)));
        else if (what_to_do == "w")
           threads.push_back(std::thread(write, std::ref(rw_mutex)));
    } while (what_to_do != "stop");

    for (auto& th : threads)
        th.join();

    return 0;
}
