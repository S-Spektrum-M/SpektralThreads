#pragma once
#include <chrono>
#include <string>

namespace Logging {
class LogItem {
    friend class Log;
private:
public:
    LogItem(std::string source, std::string message);
    ~LogItem();
    LogItem *prevNode = nullptr;
    LogItem *nextNode = nullptr;
    std::string message;
    std::string source;
    std::chrono::system_clock::time_point time;
};
}
