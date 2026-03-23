#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <memory>




#include "IDataDisplayer.hpp"
#include "__Collector/Collector.hpp"




class DataDispSFML : public IDataDisplayer {

public:

    DataDispSFML
    
        (
            std::shared_ptr<Collector> coll, 
            std::string disp_name
        );

    void init  () override;
    void update() override;
    bool isOpen() const override;

private:

    std::shared_ptr<Collector> collector;
    std::string disp_name;
    sf::RenderWindow window;
    std::shared_ptr<sf::Font> font;
    std::shared_ptr<sf::Text> text;

};
