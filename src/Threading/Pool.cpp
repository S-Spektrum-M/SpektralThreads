#include "Pool.hpp"
#include <algorithm>
#include <iostream>
using namespace Threading;
using std::thread;

Pool *Pool::inst = nullptr;

void Pool::Start(uint8_t num_threads) {
    num_threads =
        std::min((int)num_threads, (int)std::thread::hardware_concurrency() - this->started);
    this->started += num_threads;
    for (uint32_t ii = 0; ii < num_threads; ++ii) {
        threads.emplace_back(thread(&Pool::ThreadLoop, this));
    }
    std::cout << threads.size() << std::endl;
}

void Pool::ThreadLoop() {
    while (true) {
        Job job;
        std::shared_ptr<void> arg;
        std::unique_lock<std::mutex> lock(queue_mutex);
        mutex_condition.wait(
            lock, [this] { return !jobs.empty() || should_terminate; });
        if (should_terminate) {
            return;
        }
        job = jobs.front().first;
        arg = jobs.front().second;
        jobs.pop();
        job(arg);
    }
}

void Pool::QueueJob(const Job &job, std::shared_ptr<void> args) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push({job, args});
    }
    mutex_condition.notify_one();
}

bool Pool::busy() {
    bool poolBusy;
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        poolBusy = !jobs.empty();
    }
    return poolBusy;
}

void Pool::Stop(uint8_t num_threads) {
    if (num_threads > this->started) {
         throw std::invalid_argument("Cannot stop more threads than started");
         return;
    }
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();
    for (int i = threads.size() - num_threads; i < threads.size(); i++) {
        threads[i].join();
    }
    for (int i = threads.size(); i < this->started; i++) {
        threads[i].join();
    }
    this->started = std::max(0, this->started - num_threads);
}

void Pool::ForceStop() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();
    for (thread &active_thread : threads) {
        active_thread.join();
    }
    threads.clear();
    this->started = 0;
}

Pool::~Pool() {
    ForceStop();
}
