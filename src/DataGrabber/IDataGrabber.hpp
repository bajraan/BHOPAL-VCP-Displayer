#pragma once

#include "Config/InterfaceConfigLoader.hpp"

#include <string>
#include <memory>

// Wirtualny interfejs warstwy "Grabber"
class IDataGrabber {

public:
    virtual ~IDataGrabber() = default;

    virtual void interfacesInit() = 0;
    virtual void interfacesStart() = 0;
    virtual void interfacesStop() = 0;
    virtual void interfacesReset() = 0;
};