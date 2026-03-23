#include <thread>
#include <chrono>
#include <iomanip>


#include "Utils/AppEnvironment.hpp"

#include "DataGrabber/DataGrabbSerial.hpp"

#include "DataDisplayer/DataDispSFML.hpp"
#include "DataDisplayer/FactoryDisplay.hpp"
#include "__FlowPass/FlowPass.hpp"
#include "__FlowPass/FactoryFlowPass.hpp"

#include "__Core/Core.hpp"

#include "PreTest_compiler/PreTest_compiler.hpp"
#include "PreTest_sfml/PreTest_sfml.hpp"
#include "PreTest_boost/PreTest_boost.hpp"
#include "PreTest_nlohmann/PreTest_nlohmann.hpp"



// PROJECT IMPLEMENTATION STATUS
// grabberSerial implemented
// collectorSerial implemented
// regexer not impl
// flow not impl
// displayer SFML implemented
// temp displaying in main looP


// TODO: .json remove phy_name form the flowPass becase flowpass
// currently besed only on disp_name ( from disp_name to disp_name )
// BUG: brak poprawnej inicjalizacji displayera jeśli dane nie przychodzą


int main()
{


    { PreTest_compiler  pretest; }
    { PreTest_sfml      pretest; }
    { PreTest_boost     pretest; }
    { PreTest_nlohmann  pretest; }

    
    // TODO: OZNACZ JAKO KLASA singleton
    // Ładowanie konfiguracji portów (plik JSON)                        
    std::string configPath = getExecutableDir() + "/config.json";          
    auto config     = std::make_shared<InterfaceConfigLoader>(configPath);
    auto collector  = std::make_shared<Collector>();
    

    config->printSummary();

    
    Core core   ( 
                    FactoryDisplay::produce(collector,config),
                    FactoryFlowPass::produce(collector,config)
                );
    core.init();


    //===========================================================================================================
    //===========================================================================================================
    // TODO create Factory  -> IGrabber     -> GrabberSerial
    //                                      -> GrabberEthernet
    //                                      -> GrabberUSB   
    //===========================================================================================================
    // Graber danych
    //===========================================================================================================
    DataGrabbSerial grabberSerial(config,collector) ;
    grabberSerial.interfacesInit();
    // Start grabbera w osobnym wątku
    std::thread serialThread([&]()             
    {
        try
        {
            grabberSerial.interfacesStart(); // zawiera io.run()
        } catch (const std::exception& e) {
            std::cerr << "[Thread Error] " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "[Thread Error] Unknows Exception in grabber threrad.\n";
        }
    });
    //===========================================================================================================
    //===========================================================================================================

    
    for (int i = 0; i < 600; ++i) 
    {
        core.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }


    grabberSerial.interfacesStop(); // wymusza zakończenie run()
    // Dodajemy join, żeby wątek zakończył się przed końcem programu
    if (serialThread.joinable()) {
        serialThread.join();
    }
}