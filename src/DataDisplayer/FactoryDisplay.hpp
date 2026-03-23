

#include <memory>
#include <vector>


#include "../__Collector/Collector.hpp"
#include "../Config/InterfaceConfigLoader.hpp"
#include "../DataDisplayer/DataDispSFML.hpp"


class FactoryDisplay {

    FactoryDisplay() = delete;

    public:
    static std::vector<std::shared_ptr<DataDispSFML>>
    produce
    (
        std::shared_ptr<Collector>              coll,
        std::shared_ptr<InterfaceConfigLoader>  conf
    )
    {
        std::vector<std::shared_ptr<DataDispSFML>> displays;
        auto cfg = conf->getDisplays();
        for (auto& c : cfg)
        {
            displays.push_back(std::make_shared<DataDispSFML>(coll, c.disp_name));        
        }
        return displays;
    }
};