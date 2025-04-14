#include <SFML/Graphics.hpp>
#include <boost/asio.hpp>
#include <iostream>

int main()
{
    std::cout << "Boost version: " 
              << BOOST_VERSION / 100000 << "."          // major
              << BOOST_VERSION / 100 % 1000 << "."      // minor
              << BOOST_VERSION % 100                    // patch
              << std::endl;

    std::cout << "SFML version: "
              << SFML_VERSION_MAJOR << "."
              << SFML_VERSION_MINOR << "."
              << SFML_VERSION_PATCH << std::endl;
    
    sf::RenderWindow   window(sf::VideoMode({200, 200}), "BHOPAL Displayer", sf::Style::Default );
    sf::RenderWindow settings(sf::VideoMode({300, 300}), "BHOPAL Settings", sf::Style::None );

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color(255,10,10));
    
    // boost::asio::io_context io;
    // boost::asio::serial_port serial(io, "COM5");
    // //Configure basic serial port parameters: 115.2kBaud, 8N1
    // serial.set_option(boost::asio::serial_port_base::baud_rate(9600));
    // serial.set_option(boost::asio::serial_port_base::character_size(8 /* data bits */));
    // serial.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    // serial.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));

    // while(true) 
    // {
    //     char data[256];
    //     size_t n = serial.read_some(boost::asio::buffer(data, 256));
    //     // Write data to stdout
    //     std::cout.write(data, n);
    // }

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear( sf::Color(10,10,10,100) );
        window.draw(shape);
        window.display();

        settings.clear();
        settings.display();
    }
}