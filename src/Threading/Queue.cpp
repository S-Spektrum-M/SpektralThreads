#include "Queue.hpp"
#include <cstdlib>
using namespace Threading;

template <typename T>
Queue<T>::Queue(int max_size, std::function<int(int)> resize) {
    this->max_size = max_size;
    this->resize = resize;
}

template <typename T>
Queue<T>::~Queue() {
    free(data);
}

template <typename T>
T Queue<T>::front() {
    return data[0];
}

template <typename T>
T Queue<T>::pop() {
    T ret = front();
    ++data;
    free(data - 1);
    return ret;
}

template <typename T>
void Queue<T>::push(T val) {
    // If the queue is full, double the size
    if (size == max_size) {
        max_size = resize(max_size);
        T* new_data = (T*)realloc(data, max_size * sizeof(T));
        for (int i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }
        free(data);
        data = new_data;
    }
}

template <typename T>
bool Queue<T>::empty() {
    return !size;
}

