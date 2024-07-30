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

int main() {
    using namespace Threading;
    auto pool = Pool::getInst();
    Threading::Job example = [](std::shared_ptr<void> arg) {
        std::shared_ptr<Course> a = std::static_pointer_cast<Course>(arg);
        std::cout << "ID: " << a->course_id << std::endl;
        std::cout << "Department: " << a->dept << std::endl;
        std::cout << "Professor: " << a->professor << std::endl;
    };
    Course a = {"CS", "Turing", 101 , 2020};
    Course b = {"PHYS", "Newton", 102 , 2021};
    Course c = {"MATH", "Euler", 103 , 2022};
    Course d = {"PSY", "Nietzsche", 104 , 2023};
    pool->QueueJob(example, std::make_shared<Course>(a));
    pool->QueueJob(example, std::make_shared<Course>(b));
    pool->QueueJob(example, std::make_shared<Course>(c));
    pool->QueueJob(example, std::make_shared<Course>(d));
    pool->Start();
    pool->Stop();
}
