#pragma once

#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

struct InterfaceConfig {                //
    std::string interfaceType;          //
    std::string phy_name;               // phy_name
    int baud = 9600;
    int characterSize = 8;
    std::string flowControl = "none";
    std::string parity = "none";
    std::string stopBits = "one";
};

struct FlowConfiguration {
    std::string phy_name;   // from which phy interface
    std::string disp_name;  // to which display
    std::string pattern;    // first stage regex aka pattern
    std::string regex;      // second stage regex
    std::string prefix;     // prefix displaying on display
    std::string postfix;    // postfix dispalying on display
    int    datasize;
};

struct DisplayConfig {
    std::string disp_name;
};

class InterfaceConfigLoader {

public:

    InterfaceConfigLoader(const std::string& path);

    const std::vector<InterfaceConfig>   & getInterfaces() const;
    const std::vector<FlowConfiguration> & getFlowPass() const;
    const std::vector<DisplayConfig>     & getDisplays() const;
          std::vector<std::string>         getDisplaysConnectedToPhyName( std::string );

    void printSummary();


private:

    std::vector<InterfaceConfig>    interfaces;
    std::vector<FlowConfiguration>  flowpass;
    std::vector<DisplayConfig>      displays;

    // TODO: better name for this map
    //       phy_name                 disp_name
    std::map<std::string, std::vector<std::string>> matrix;

    void parseInterfaces(const nlohmann::json& json);
    void parseFlowPass(const nlohmann::json& json);
    void parseDisplays(const nlohmann::json& json);
    
    void initMatrix();
    bool loadFromFile(const std::string& path);

};