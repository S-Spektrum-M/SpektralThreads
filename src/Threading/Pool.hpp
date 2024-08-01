#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include "Queue.hpp"

namespace Threading {
// return void, accept
// void * to be typecast
// in lambda body
using Job = std::function<void(std::shared_ptr<void>)>;
class Pool {
  public:
    /*
     *  Starts the thread pool with the specified number of threads. If
     *  the number is not explicitly defined, it will default to the
     *  number of hardware threads available. If the number of threads
     *  is greater than the number of hardware threads, it will default
     *  to the number of hardware threads.
     *
     *  @param num_threads: The number of threads to start
     *  @return void
     *  @see Threading::Pool::Stop()
     *  @see Threading::Pool::started
     *  @see std::thread::hardware_concurrency()
     */
    void Start(uint8_t num_threads = std::thread::hardware_concurrency());
    /*
     * Stops the thread pool. If the number of threads is not explicitly
     * defined, it will default to the number of hardware threads
     * available. If the number of threads is greater than the number of
     * threads already started, it will default to the number of threads
     * already started.
     *
     * @param num_threads: The number of threads to stop
     * @return void
     * @see Threading::Pool::Start()
     * @see Threading::Pool::started;
     * @see std::thread::hardware_concurrency
     */
    void Stop(uint8_t num_threads = std::thread::hardware_concurrency());
    /*
     * Add a job to the queue. The job will be executed by the next
     * available thread. The job will be executed with the arguments
     * passed in the second parameter.
     *
     * @param job: The job to be executed
     * @param args: The arguments to be passed to the job
     */
    void QueueJob(const Job &job, std::shared_ptr<void> args);
    /*
     * Forcefully stop the thread pool. This will stop all threads
     * immediately and clear the job queue.
     *
     * @return void
     * @see Threading::Pool::Stop()
     */
    void ForceStop();
    /*
     * Returns true if the Job queue is not empty.
     *
     * @return bool
     * @see Threading::Pool::jobs
     */
    inline bool busy();
    /*
     * Returns the singleton instance of the Pool class. If the instance
     * does not exist, it will create one.
     *
     * @return Pool *
     */
    static Pool *getInst() {
        if (inst == NULL) {
            inst = new Pool();
        }
        return inst;
    }
    /*
     * Returns the ids of all threads curretnly started in the pool.
     *
     * @return std::vector<std::thread::id>
     * @see std::thread::id
     */
    std::vector<std::thread::id> getIds();
    ~Pool();

  private:
    // Singleton instance
    static Pool *inst;
    /*
     * The number of threads currently started in the pool.
     *
     * @see Threading::Pool::Start()
     * @see Threading::Pool::Stop()
     */
    int started;
    /*
     * The function that each thread will run. This function will wait for a
     * condition variable to be notified, then execute the next job in the
     * queue.
     */
    void ThreadLoop();
    bool should_terminate; // Tells threads to stop looking for jobs
    std::mutex queue_mutex;        // Prevents data races to the job queue
    std::condition_variable
        mutex_condition; // Allows threads to wait on new jobs or termination
    /* The threads in the pool
     * @see Threading::Pool::Start()
     * @see Threading::Pool::Stop()
     * @see Threading::Pool::ThreadLoop()
     * @see Threading::Pool::ForceStop()
     */
    std::vector<std::thread> threads;
    /* The job queue
     * @see Threading::Pool::QueueJob()
     * @see Threading::Pool::ThreadLoop()
     * @see Threading::Pool::busy()
     * @see Threading::Pool::ForceStop()
     */
    Queue<std::pair<Job, std::shared_ptr<void>>> jobs;
    // std::queue<std::pair<Job, std::shared_ptr<void>>> jobs;
    Pool();
};
} // namespace Threading
