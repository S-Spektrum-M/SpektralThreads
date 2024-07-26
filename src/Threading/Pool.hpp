#include <functional>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <thread>
#include <queue>

namespace Threading {
using Job = std::function<void(std::shared_ptr<void>)>; // return void, accept
                                                        // void * to be typecast
                                                        // in lambda body
class Pool {
public:
    // Start the Pool
    void Start(unsigned int num_threads=std::thread::hardware_concurrency());
    // Add a Job(std::function<void()>&) to the Job Queue
    void QueueJob(const Job& job, std::shared_ptr<void> args);
    // Wait for all threads to finish and join
    void Stop();
    // End all threads
    void ForceStop();
    // isBusy?
    bool busy();
    static Pool * getInst() {
        if (inst == NULL) {
            inst = new Pool();
        }
        return inst;
    }
    ~Pool();

private:
    static Pool * inst;
    void ThreadLoop();
    bool should_terminate = false;           // Tells threads to stop looking for jobs
    std::mutex queue_mutex;                  // Prevents data races to the job queue
    std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination
    std::vector<std::thread> threads;
    // std::queue<Job> jobs;
    std::queue<std::pair<Job, std::shared_ptr<void>>> jobs;
    Pool() {}
};
}
