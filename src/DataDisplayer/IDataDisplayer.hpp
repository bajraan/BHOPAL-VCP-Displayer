// IDataDisplayer.hpp
#pragma once
#include <memory>

class IDataCollector; // forward declaration

class IDataDisplayer {
public:
    virtual ~IDataDisplayer() = default;
    virtual void init() = 0;
    virtual void update() = 0;
    virtual bool isOpen() const = 0;
};
