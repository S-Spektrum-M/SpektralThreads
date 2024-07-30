#include "instr.hpp"
#include <iostream>

Threading::instr *Threading::instr::inst = nullptr;
Threading::instr::instr(std::istream &is) : is_(is), mutex_() {}

size_t Threading::instr::read(char *data, size_t len) {
    std::lock_guard<std::mutex> lock(mutex_);
    is_.read(data, len);
    return is_.gcount();
}

Threading::instr *Threading::instr::getInst() {
    if (inst == nullptr) {
        inst = new instr(std::cin);
    }
    return inst;
}
