#include "Pool.hpp"
#include <algorithm>
#include <iostream>
using namespace Threading;
using std::thread;

Pool *Pool::inst = nullptr;

void Pool::Start(unsigned int num_threads) {
    std::cout << num_threads << std::endl;
    for (uint32_t ii = 0; ii < num_threads; ++ii) {
        threads.emplace_back(thread(&Pool::ThreadLoop, this));
    }
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

void Pool::Stop() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();
    for (thread &active_thread : threads) {
        active_thread.join();
    }
    threads.clear();
    started = false;
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
    started = false;
}

Pool::~Pool() {
    ForceStop();
    while (!jobs.empty()) {
    }
}
