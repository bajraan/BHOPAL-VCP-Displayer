#pragma once

#include <boost/asio.hpp>

#include <map>
#include <deque>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "IDataGrabber.hpp"
#include "Config/InterfaceConfigLoader.hpp"
#include "__Collector/Collector.hpp"

// Implementacja dla portów szeregowych
class DataGrabbSerial : public IDataGrabber {

public:
    DataGrabbSerial() = delete;
    DataGrabbSerial(std::shared_ptr<InterfaceConfigLoader> config, std::shared_ptr<Collector> collector );
    ~DataGrabbSerial();
  
    void interfacesInit() override;
    void interfacesStart() override;
    void interfacesStop() override;
    void interfacesReset() override;

private:

    std::shared_ptr<Collector> collector;
    std::shared_ptr<InterfaceConfigLoader> config;

    boost::asio::io_context io;

    struct SerialInterface {
        std::unique_ptr<boost::asio::serial_port> port;
        boost::asio::streambuf buffer;

        // Konstruktory przenoszące (move) dla bezproblemowego umieszczania w strukturach
        SerialInterface() = default;
        SerialInterface(SerialInterface&& other) noexcept
            : port(std::move(other.port))
        {
            // Bufor nie jest przenoszony (pozostaje pusty, dane z other.buffer zostają utracone)
        }
        
        SerialInterface& operator=(SerialInterface&& other) noexcept
        {
            if (this != &other) {
                port = std::move(other.port);
                // Wyczyść aktualny bufor (nie przenosimy danych z other.buffer)
                buffer.consume(buffer.size());
            }
            return *this;
        }

        // Usunięte konstruktory kopiujące (unique_ptr nie pozwala na kopiowanie)
        SerialInterface(const SerialInterface&) = delete;
        SerialInterface& operator=(const SerialInterface&) = delete;
    };

    std::map<std::string, SerialInterface> serialInterfaces;

    void setupSerialPort(const InterfaceConfig& cfg);
    void startAsyncRead(const std::string& name);

    void handleRead(
        const std::string& name,
        const boost::system::error_code& ec,
        std::size_t bytes_transferred
    );
};