#include "Pool.hpp"
using namespace Threading;
using std::thread;

void Pool::Start() {
    const auto num_threads = thread::hardware_concurrency(); // Max # of threads the system supports
    for (uint32_t ii = 0; ii < num_threads; ++ii) {
        threads.emplace_back(thread(&Pool::ThreadLoop,this));
    }
}

void Pool::ThreadLoop() {
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            mutex_condition.wait(lock, [this] {
                return !jobs.empty() || should_terminate;
            });
            if (should_terminate) {
                return;
            }
            job = jobs.front();
            jobs.pop();
        }
        job();
    }
}

void Pool::QueueJob(const std::function<void()>& job) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push(job);
    }
    mutex_condition.notify_one();
}
// threadPoolInst->QueueJob([] { /* ... */ });

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
    for (thread& active_thread : threads) {
        active_thread.join();
    }
    threads.clear();
}

void Pool::ForceStop() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();
    for (thread& active_thread : threads) {
        active_thread.join();
    }
    threads.clear();
}
