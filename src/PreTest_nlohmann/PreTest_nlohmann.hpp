# pragma once

//
// header only
//



#include <iostream>



class PreTest_nlohmann {

public:
    PreTest_nlohmann(void)
    {
        std::cout   << "nlohmann/json version: "
                    << NLOHMANN_JSON_VERSION_MAJOR << "."
                    << NLOHMANN_JSON_VERSION_MINOR << "."
                    << NLOHMANN_JSON_VERSION_PATCH << std::endl;
    }
    
};