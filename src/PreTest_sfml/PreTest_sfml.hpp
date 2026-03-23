# pragma once

//
// header only
//
#include <SFML/Config.hpp>


#include <iostream>


class PreTest_sfml {

public:
    PreTest_sfml(void)
    {
        std::cout   << "SFML version: "
                    << SFML_VERSION_MAJOR << "."
                    << SFML_VERSION_MINOR << "."
                    << SFML_VERSION_PATCH << std::endl;
    }
};