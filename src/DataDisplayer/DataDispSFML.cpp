
#include "Utils/AppEnvironment.hpp"
#include "DataDispSFML.hpp"

#include <algorithm>
#include <string>
#include <cctype>
#include <iostream>

using namespace sf;

DataDispSFML::DataDispSFML
(  
    std::shared_ptr<Collector> cl,
    std::string name   
) : collector(cl), disp_name(name)  
{}

void DataDispSFML::init() {
    Vector2u size(300, 100);
    window.create(VideoMode(size), disp_name, Style::Titlebar | Style::Close);
    window.setFramerateLimit(30);

    std::string fontPath = getExecutableDir() + "\\..\\res\\JetBrainsMono-Medium.ttf";
    font = std::make_shared<sf::Font>(fontPath);
    text = std::make_shared<sf::Text>(*font, "TEST", 24);
    text->setFillColor(Color::Green);
    Vector2f pos(10.f, 30.f);
    text->setPosition(pos);
}

void DataDispSFML::update() {

    if (!window.isOpen() || !collector || !text) return;
    
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
    
    auto data = collector->buffer_Disp().try_pop(disp_name);
    if (data) 
    {
        text->setString( data.value() );
        window.clear(Color::Black);
        window.draw(*text);
        window.display();
    }
}

bool DataDispSFML::isOpen() const {
    return window.isOpen();
}
