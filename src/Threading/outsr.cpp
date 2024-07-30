#include "outsr.hpp"
#include <iostream>

Threading::outsr * Threading::outsr::inst = nullptr;

Threading::outsr::outsr(std::ostream& os) : os_(os), mutex_() {}

void Threading::outsr::write(const char* data, size_t len) {
    std::lock_guard<std::mutex> lock(mutex_);
    os_.write(data, len);
}

void Threading::outsr::flush() {
    std::lock_guard<std::mutex> lock(mutex_);
    os_.flush();
}

Threading::outsr * Threading::outsr::getInst() {
    if (inst == nullptr) {
        inst = new outsr(std::cout);
    }
    return inst;
}
