#pragma once
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <stack>
#include "LogItem.hpp"

namespace Logging{
class Log {
private:
    Log();
    ~Log();
    static Log *inst;
    int maxDepth;
    std::vector<LogItem> knownNodes;
    void rebalanceHelper(std::vector<LogItem *> *list, LogItem *node);
    void rebalanceInsertion(std::vector<LogItem *> *list, LogItem *node);
    void rebalanceInsertion(std::vector<LogItem *> *l, int start, int end);
    void print(std::ofstream *file, LogItem *node);
    void rebalance();

public:
    LogItem *root = nullptr; //NOTE: Move back to private after done debugging
    static Log *getInstance();
    void insertItem(std::string source, std::string message);
    void print(std::string fileName);
};
}
