#include <functional>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <thread>
#include <queue>

namespace Threading {
class Pool {
public:
    // Start the Pool
    void Start();
    // Add a Job(std::function<void()>&) to the Job Queue
    void QueueJob(const std::function<void()>& job);
    // Wait for all threads to finish and join
    void Stop();
    // End all threads
    void ForceStop();
    // isBusy?
    bool busy();
    static Threading::Pool * inst;
    static Threading::Pool * getInst() {
        if (inst == nullptr) inst = new Threading::Pool();
        return inst;
    }

private:
    void ThreadLoop();
    bool should_terminate = false;           // Tells threads to stop looking for jobs
    std::mutex queue_mutex;                  // Prevents data races to the job queue
    std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;
};
}
