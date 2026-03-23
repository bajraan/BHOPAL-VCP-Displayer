#pragma once
// ==============================================
// Flower and Parser aka " Filtr przepływu "
// ==============================================

#include <format>
#include <memory>
#include <vector>
#include <regex>

#include "../__Collector/Collector.hpp"
#include "../Config/InterfaceConfigLoader.hpp"

class FlowPass {

    std::shared_ptr<Collector>          collector;
    std::shared_ptr<FlowConfiguration>  config;

    std::regex compiled_pattern;
    std::regex compiled_regex;

public:

   ~FlowPass()                              = default;
    FlowPass()                              = delete;
    FlowPass( const FlowPass& )             = delete;
    FlowPass( FlowPass&& )                  = delete;
    FlowPass& operator= ( const FlowPass& ) = delete;
    FlowPass& operator= ( FlowPass&& )      = delete;

    FlowPass
    (
            std::shared_ptr<Collector> col,
            std::shared_ptr<FlowConfiguration> cfg
    ) 
        :   collector(col), 
            config(cfg),
            compiled_pattern(cfg ? std::regex(cfg->pattern) : std::regex()),    // high cost of initalizing regex
            compiled_regex(cfg ? std::regex(cfg->regex) : std::regex())         // regex initliaziation only in constructor not in run()
    {
        if(!collector)
        {
            std::cout << " [FATAL] in: FlowPass() " << std::endl;
            std::cout << " [FATAL] collector pointer is empty" << std::endl;
        }
        if(!config)
        {
            std::cout << " [FATAL] in: FlowPass() " << std::endl;
            std::cout << " [FATAL] config pointer is empty" << std::endl;
        }
    }

    void update() 
    {
        auto data = collector->buffer_Grab().try_pop(config->disp_name);
        if(!data) return;
            
        std::smatch match;

        // Stage 1: input validation
        //const std::regex pattern(config->pattern);
        //if ( ! std::regex_match (data.value(), pattern)) return;
        // Stage 2: field extraction
        //const std::regex extract(config->regex);
        //if ( ! std::regex_search(data.value(), match, extract)) return;


        if (!std::regex_match(*data, compiled_pattern)) return;
        if (!std::regex_search(*data, match, compiled_regex)) return;

        std::string result = match[1];

        // std::cout << "  == --- reg stage --- == "        << std::endl;
        // std::cout << "phy_name : " << config->phy_name   << std::endl;
        // std::cout << "disp_name: " << config->disp_name  << std::endl;
        // std::cout << "input    : " << data.value()       << std::endl;
        // std::cout << "pattern  : " << config->pattern    << std::endl;
        // std::cout << "regex    : " << config->regex      << std::endl;
        // std::cout << "match[0] : " << match[0]           << std::endl;
        // std::cout << "match[1] : " << match[1]           << std::endl;
        // std::cout << "match[2] : " << match[2]           << std::endl;
        // std::cout << "result   : " << result             << std::endl;
        // std::cout                                        << std::endl;

        auto datatodisplay = format("{}{:>{}}{}", config->prefix, result, config->datasize, config->postfix);
        collector->buffer_Disp().push(config->disp_name, datatodisplay );
    }
};
