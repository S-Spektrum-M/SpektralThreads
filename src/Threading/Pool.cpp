#include "Pool.hpp"
#include <algorithm>
#include <future>
#include <iostream>
#include <unistd.h>
using namespace Threading;
using std::thread;

// Threading::Pool implentations
Pool *Pool::inst = nullptr;

void Pool::Start(uint8_t num_threads) {
    if (this->started > 0 &&
        this->started < (int)thread::hardware_concurrency()) {
        std::future<void> handle =
            std::async(std::launch::async, [&num_threads, this]() {
                num_threads = std::min((int)num_threads,
                                       (int)thread::hardware_concurrency() -
                                           this->started);
                this->started += num_threads;
                for (uint8_t ii = 0; ii < num_threads; ++ii) {
                    threads.emplace_back(thread(&Pool::ThreadLoop, this));
                }
            });
        return handle.get();
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

inline bool Pool::busy() {
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
    for (int i = threads.size() - num_threads; i < (int)threads.size(); i++)
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

Pool::Pool() {
    this->started = 0;
    this->should_terminate = false;
    // this->jobs = std::queue<std::pair<Job, std::shared_ptr<void>>>();
    this->jobs = Queue<std::pair<Job, std::shared_ptr<void>>>(
        1, [](int x) { return x * 2; });
}

Pool::~Pool() {
    // wait for all threads to finish then join then delete
    while (busy()) {
    }
    Pool::inst = nullptr;
}

// Threading::Queue implentations
template <typename T>
Queue<T>::Queue(int max_size, std::function<int(int)> resize) {
    this->data = (T *)std::calloc(0, max_size * sizeof(T));
    if (this->data == nullptr) throw std::bad_alloc();
    this->max_size = max_size;
    this->resize = resize;
}

template <typename T> Queue<T>::~Queue() {
}

template <typename T> T Queue<T>::front() {
    try {
        return data[0];
    } catch (std::exception &e) {
        std::cerr << "Exception in jobs.front()" << std::endl;
    }
    return T();
}

template <typename T> T Queue<T>::pop() {
    T ret = front();
    ++data;
    free(data - 1);
    return ret;
}

template <typename T> void Queue<T>::push(T val) {
    // If the queue is full, double the size
    if (size == max_size) {
        max_size = resize(max_size);
        if (data != nullptr) {
            data = (T *)std::realloc(data, max_size * sizeof(T));
            if (data == nullptr) throw std::bad_alloc();
        } else {
            data = (T *)std::malloc(max_size * sizeof(T));
        }
        /* T* new_data = (T*)std::malloc(max_size * sizeof(T));
        for (int i = 0; i < max_size; ++i) {
            new_data[i] = data[i];
        }
        free(data); */
        // data = new_data;
    }
    try {
        std::cout << data << std::endl;
        data[size++] = val;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

template <typename T> bool Queue<T>::empty() { return !size; }
