#include <functional>

namespace Threading {
// A cahce safe queue implementation
template <typename T>
class Queue {
private:
    int size;
    int max_size;
    std::function<int(int)> resize;
    T *data;
public:
    /* Constructor
    *
    * @param max_size: The initial maximum size of the queue; default is 1
    * @param resize: A function that takes an integer and returns an integer;
    * default is doubling the size
    */
    Queue(int max_size = 1, std::function<int(int)> resize = [](int x) {return x * 2;});
    /* Destructor
    * Frees the memory allocated for the queue data
    */
    ~Queue();
    /*
    * Returns the front element of the queue
    * @return T
    */
    T front();
    /*
    * Returns the front element of the queue and removes it
    * @return T
    */
    T pop();
    /*
    * Pushes a value to the back of the queue
    * @param val: The value to be pushed
    * @return void
    */
    void push(T val);
    /* Returns true if the queue is empty
    * @return bool
    */
    bool empty();
};
}
