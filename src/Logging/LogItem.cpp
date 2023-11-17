#include "LogItem.hpp"

Logging::LogItem::~LogItem() {}

Logging::LogItem::LogItem(std::string source, std::string message) {
    this->time = std::chrono::system_clock::now();
    this->source = source;
    this->message = message;
}
