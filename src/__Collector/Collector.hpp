#pragma once


// ====================================================================
// Collector – jeden obiekt, dwa bufory
// ====================================================================


#include <map>
#include <queue>
#include <string>
#include <mutex>
#include <iostream>


class Buffer {

    std::map<std::string, std::deque<std::string>> data;
    mutable std::mutex mtx;

    bool empty_unlocked(const std::string& port) const {
        auto it = data.find(port);
        return it == data.end() || it->second.empty();
    }

public:

    void push(const std::string& port, const std::string& value) {
        std::lock_guard<std::mutex> lock(mtx);
        data[port].push_back(value);
    }

    std::optional<std::string> try_pop(const std::string& port) {
        std::lock_guard<std::mutex> lock(mtx);
        if (empty_unlocked(port)) return std::nullopt;
        std::string val = data[port].front();
        data[port].pop_front();
        return val;
    }

};




class Collector 
{
    Buffer buffer__Grabbers;
    Buffer buffer__Displays;

public:
    Buffer& buffer_Grab() { return buffer__Grabbers; }
    Buffer& buffer_Disp() { return buffer__Displays; }
};
// ====================================================================
// ====================================================================