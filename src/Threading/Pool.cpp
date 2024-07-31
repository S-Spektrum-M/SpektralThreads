#include "Pool.hpp"
#include <algorithm>
#include <unistd.h>
using namespace Threading;
using std::thread;

Pool *Pool::inst = nullptr;

void Pool::Start(uint8_t num_threads) {
    if (this->started > 0 && this->started < thread::hardware_concurrency()) {
        int delete_position = started;
        threads.emplace_back(thread([&num_threads, this](){
            num_threads =
                std::min((int)num_threads,
                         (int)thread::hardware_concurrency() - this->started);
            this->started += num_threads;
            for (uint8_t ii = 0; ii < num_threads; ++ii) {
                threads.emplace_back(thread(&Pool::ThreadLoop, this));
            }
        }, this));
        threads[delete_position].join();
        threads.erase(threads.begin() + delete_position);
    } else {
        num_threads =
            std::min((int)num_threads,
                     (int)thread::hardware_concurrency() - this->started);
        this->started += num_threads;
        for (uint8_t ii = 0; ii < num_threads; ++ii) {
            threads.emplace_back(thread(&Pool::ThreadLoop, this));
        }
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

void Pool::Stop(uint8_t num_threads) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();
    // wait for all threads to finish
    for (int i = threads.size() - num_threads; i < threads.size(); i++)
        threads[i].join();
    threads.erase(threads.end() - num_threads, threads.end());
    this->started = std::max(0, this->started - num_threads);
}

std::vector<thread::id> Pool::getIds() {
    std::vector<thread::id> ids;
    for (thread &t : threads) {
        ids.push_back(t.get_id());
    }
    return ids;
}

void Pool::ForceStop() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();
    for (thread &active_thread : threads) {
        active_thread.detach();
        delete &active_thread;
    }
    threads.clear();
    this->started = 0;
}

Pool::~Pool() {
    // wait for all threads to finish then join then delete
    while(busy()) {}
    Pool::inst = nullptr;
}
