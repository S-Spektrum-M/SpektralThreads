#include "Log.hpp"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using std::string;
using std::vector;

Logging::Log::Log() {}
Logging::Log::~Log() {}
Logging::Log *Logging::Log::inst = nullptr;

Logging::Log *Logging::Log::getInstance() {
    if (Logging::Log::inst) {
        return inst;
    }
    inst = new Logging::Log();
    return inst;
}

void Logging::Log::insertItem(std::string source, std::string message) {
    if (root) {
        auto time = std::chrono::system_clock::now();
        if (maxDepth > 10)
            rebalance();
        LogItem * curr = root;
        int depth = 1;
        while (curr != nullptr) {
            if (time >= curr->time) {
                curr = curr->nextNode;
            } else {
                curr = curr->prevNode;
            }
            ++depth;
        }
        knownNodes.push_back(LogItem(source, message));
        curr = &(knownNodes.back());
        maxDepth = std::max(depth, maxDepth);
    } else {
        knownNodes.push_back(LogItem(source, message));
        root = &(knownNodes.back());
    }
}

void Logging::Log::rebalanceHelper(vector<LogItem *> *l, LogItem *node) {
    if (node == nullptr)
        return;
    rebalanceHelper(l, node->prevNode);
    l->push_back(node);
    rebalanceHelper(l, node->nextNode);
}

void Logging::Log::rebalanceInsertion(vector<LogItem *> *l, int start, int end) {
    if (start == end)
        return;
    int middle = (start + end) / 2;
    auto n = l->at(middle);
    insertItem(n->source, n->message);
    rebalanceInsertion(l, start, middle);
    rebalanceInsertion(l, middle + 1, end);
}

void Logging::Log::rebalance() {
    vector<LogItem *> items;
    rebalanceHelper(&items, root);
    root = nullptr;
    maxDepth = 0;
    rebalanceInsertion(&items, 0, items.size());
}

void Logging::Log::print(string fileName) {
    std::ofstream file;
    file.open(fileName);
    print(&file, root);
    file.close();
}

void Logging::Log::print(std::ofstream *file, LogItem * node) {
    if (root == nullptr) return;
    print(file, root->prevNode);
    auto time = std::chrono::system_clock::to_time_t(node->time);
    (*file) << "[" << ctime(&time) << "]" << " " << node->source << ":    " << node->message << "\n";
    print(file, root->nextNode);
}
