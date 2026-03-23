#include "InterfaceConfigLoader.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>

using json = nlohmann::json;

InterfaceConfigLoader::InterfaceConfigLoader(const std::string &path)
{
    if (!this->loadFromFile(path)) {
        std::cerr << "[Error] Failed to load interface config.\n";
    } else{
        std::cout << "[ OK ] Concifuratin loaded : "      << std::endl;
        std::cout << " phy amount: " << interfaces.size() << std::endl;
    }

    this->initMatrix();

}

bool InterfaceConfigLoader::loadFromFile(const std::string &path)
{
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Could not open config file: " << path << std::endl;
        return false;
    }

    json j;
    try {
        file >> j;
        parseInterfaces(j);
        parseFlowPass(j);
        parseDisplays(j);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing config: " << e.what() << std::endl;
        return false;
    }
    return true;
}

void InterfaceConfigLoader::parseInterfaces(const json& j)
{
    interfaces.clear();
    if (!j.contains("interfaces"))
    {
        std::cout << "InterfaceConfigLoader::parseInterfaces(const json& j)" << std::endl;
        std::cout << "[ WARNING ] empty Interfaces in config.json"           << std::endl;
        return;
    }

    for (const auto& entry : j["interfaces"]) {
        InterfaceConfig config;
        config.interfaceType = entry.value("interfaceType", "");
        config.phy_name = entry.value("name", "");

        if (entry.contains("parameters")) {
            const auto& params = entry["parameters"];
            config.baud          = params.value("baud", 9600);
            config.characterSize = params.value("characterSize", 8);
            config.flowControl   = params.value("flowControl", "none");
            config.parity        = params.value("parity", "none");
            config.stopBits      = params.value("stopBits", "one");
        }
        interfaces.push_back(config);
    }
}

void InterfaceConfigLoader::parseFlowPass(const json& j)
{
    flowpass.clear();
    if (!j.contains("flowpass"))
    {
        std::cout << "InterfaceConfigLoader::parseGrabbers(const json& j)" << std::endl;
        std::cout << "[ WARNING ] empty FlowPass in config.json"           << std::endl;
        return;
    }

    for (const auto& entry : j["flowpass"]) {
        FlowConfiguration config;

        std::string name = entry.value("name", "");

        if (entry.contains("parameters")) {
            const auto& params = entry["parameters"];
            config.phy_name         = params.value("phy_name", "");
            config.disp_name        = params.value("disp_name","");
            config.pattern          = params.value("PATTERN","");
            config.regex            = params.value("REGEX", "");
            config.prefix           = params.value("prefix","");
            config.postfix          = params.value("postfix","");
        }

        flowpass.push_back(config);
    }
}

void InterfaceConfigLoader::parseDisplays(const nlohmann::json &j)
{
    displays.clear();
    if (!j.contains("displays"))
    {
        std::cout << "InterfaceConfigLoader::parseDisplays(const json& j)" << std::endl;
        std::cout << "[ WARNING ] empty displays in config.json"           << std::endl;
        return;
    }

    for (const auto& entry : j["displays"]) {
        DisplayConfig config;
        config.disp_name = entry.value("disp_name", "");

        // if (entry.contains("parameters")) {
        //     const auto& params = entry["parameters"];
        //     config.phy_name         = params.value("phy_name", "");
        //     config.disp_name        = params.value("disp_name","");
        //     config.pattern          = params.value("PATTERN","");
        //     config.regex            = params.value("REGEX", "");
        // }

        displays.push_back(config);
    }
}

void InterfaceConfigLoader::initMatrix
()
{
    for(auto i : this->flowpass ){
        this->matrix[i.phy_name].push_back(i.disp_name);
    }
}

const std::vector<InterfaceConfig>& InterfaceConfigLoader::getInterfaces
()  const {
    return interfaces;
}

std::vector<std::string> InterfaceConfigLoader::getDisplaysConnectedToPhyName(std::string _phy_name)
{
    auto it = this->matrix.find(_phy_name);
    return (it!=matrix.end()) ? it->second : std::vector<std::string>{};
}

const std::vector<FlowConfiguration>& InterfaceConfigLoader::getFlowPass
()  const {
    return flowpass;
}

const std::vector<DisplayConfig>& InterfaceConfigLoader::getDisplays
()  const {
    return displays;
}

void InterfaceConfigLoader::printSummary
()
{
    std::cout << "Loaded interfaces:\n";
    for (const auto& i : interfaces)
        std::cout << " - " << i.phy_name << " (" << i.interfaceType << ")\n";
    
    std::cout << "Loaded flowpass:\n";
    for (const auto& f : flowpass){
        std::cout << " - " << std::setw(12) << "----" << " -> " << f.phy_name   << "\n";
        std::cout << " -             "                  << " -> " << f.disp_name  << "\n";
        std::cout << " - RAW pattern "                  << " -> " << f.pattern    << "\n";
        std::cout << " - RAW regex   "                  << " -> " << f.regex      << "\n";
    }

    std::cout << "Loaded displays:\n";
    for (const auto& d : displays)
        std::cout << " - " << d.disp_name << std::endl;

    std::cout << "matrix:" << std::endl;
    for(const auto& [phy, disps] : matrix)
    {
        std::cout << phy << " : "; for(const auto& d : disps) std::cout << d << " "; std::cout << std::endl;
    }
}
