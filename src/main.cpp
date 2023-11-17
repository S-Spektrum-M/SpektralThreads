#include <iostream>
using std::cout;
using std::endl;
#include "Logging/Log.hpp"
#include "Logging/LogItem.hpp"

int main() {
    Logging::Log * lg = Logging::Log::getInstance();
    cout << &(lg->root->message) << endl;
    lg->insertItem("main", "TEST");
    lg->insertItem("main", "TEST");
    lg->insertItem("main", "TEST");
    lg->insertItem("main", "TEST");
    lg->insertItem("main", "TEST");
}
