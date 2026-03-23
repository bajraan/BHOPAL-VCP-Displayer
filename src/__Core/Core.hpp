

#include <vector>
#include <thread>

#include "../Config/InterfaceConfigLoader.hpp"
#include "../DataDisplayer/DataDispSFML.hpp"
#include "../__FlowPass/FlowPass.hpp"

class Core {    

    std::vector<std::shared_ptr<DataDispSFML>>  displays;
    std::vector<std::shared_ptr<FlowPass>>      flowpass;
    
public:

    Core
    (
        std::vector<std::shared_ptr<DataDispSFML>>  _displays,
        std::vector<std::shared_ptr<FlowPass>>      _flowpass
    ) :   
            displays(std::move(_displays)), 
            flowpass(std::move(_flowpass))
    {};


    void init()
    {
        for(auto disp : displays) disp->init();
    }


    void run()
    {
        for(auto flow : flowpass ) flow->update(); 
        for(auto disp : displays ) disp->update(); 
    }

    
};