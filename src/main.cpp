#include "Threading/Pool.hpp"
#include <iostream>
#include <unistd.h>


int main() {
    // Sample usage of thread pool
    auto pool = Threading::Pool::getInst();
    pool->Start(2);
    Threading::Job example = [](std::shared_ptr<void> arg){
        for (int i = 0; i < 100 ; ++i) {
            std:: cout << "Hello from thread: " << std::this_thread::get_id() << std::endl;
        }
    };
    for (int i = 0; i < 2; ++i) {
        pool->QueueJob(example, nullptr);
    }
    pool->Stop(2);
}
