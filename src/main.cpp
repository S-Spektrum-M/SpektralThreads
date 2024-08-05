// Sample usage of thread pool

#include "Threading/Pool.hpp"
#include <iostream>
#include <unistd.h>

struct Course {
    std::string dept;
    std::string professor;
    int course_id;
    int year;
};

std::string randomString(int len) {
    // Generate random string of length len
    std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string new_str;
    for (int i = 0; i < len; i++) {
        new_str += str[rand() % str.size()];
    }
    return new_str;
}

int main() {
    using namespace Threading;
    Pool *pool = Pool::getInst();
    Threading::Job example = [](std::shared_ptr<void> arg) {
        std::shared_ptr<Course> a = std::static_pointer_cast<Course>(arg);
        std::cout << "ID: " << a->course_id << std::endl;
        std::cout << "Department: " << a->dept << std::endl;
        std::cout << "Professor: " << a->professor << std::endl;
        std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
    };
    pool->Start(8);
    for (int i = 0; i < 1000000; i++) {
        Course Rand = {randomString(3), randomString(5), i, 2020 + rand() % 5};
        pool->QueueJob(example, std::make_shared<Course>(Rand));
    }
    pool->Stop(8);
}
