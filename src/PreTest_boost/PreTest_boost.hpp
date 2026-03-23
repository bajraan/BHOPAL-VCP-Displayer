#pragma once

//
// header only
//

#include <boost/asio.hpp>

#include <iostream>



class PreTest_boost {

public:
    PreTest_boost(void)
    {
        std::cout   << "Boost version: " 
                    << BOOST_VERSION / 100000 << "."          // major
                    << BOOST_VERSION / 100 % 1000 << "."      // minor
                    << BOOST_VERSION % 100                    // patch
                    << std::endl;
    }
};