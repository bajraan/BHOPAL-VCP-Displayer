

#include <memory>
#include <vector>


#include "../__Collector/Collector.hpp"
#include "../__FlowPass/FlowPass.hpp"
#include "../Config/InterfaceConfigLoader.hpp"
#include "../DataDisplayer/DataDispSFML.hpp"


class FactoryFlowPass {

    FactoryFlowPass() = delete;

    public:
    static std::vector<std::shared_ptr<FlowPass>>
    produce
    (
        std::shared_ptr<Collector>              coll,
        std::shared_ptr<InterfaceConfigLoader>  conf
    )
    {
        std::vector<std::shared_ptr<FlowPass>> flowpass;
        for (const auto& config : conf->getFlowPass())
        {
            flowpass.push_back(std::make_shared<FlowPass>
                (
                    coll,
                    std::make_shared<FlowConfiguration>(config)
                )
            );
        }
        return flowpass;
    }
};