#include "DataGrabber/DataGrabbSerial.hpp"
#include <boost/asio/serial_port_base.hpp>
#include "DataGrabbSerial.hpp"

using namespace boost::asio;

DataGrabbSerial::DataGrabbSerial    
(
  std::shared_ptr<InterfaceConfigLoader> config, 
  std::shared_ptr<Collector> collector
)
{

    if (!config)
    {
        std::cerr << "[Error] Failed to load config.\n";
        std::cerr << "[Error] [in:] DataGrabbSerial::DataGrabbSerial\n";
    } 
    else 
    { 
        this->config = config; 
    }

    if (!collector)
    {
        std::cerr << "[Error] Failed to load DataCollector.\n";
        std::cerr << "[Error] [in:] DataGrabbSerial::DataGrabbSerial\n";
    }
    else 
    { 
        this->collector = collector; 
    }
}

DataGrabbSerial::~DataGrabbSerial
()
{
    interfacesStop();
}

void DataGrabbSerial::interfacesInit
()
{
    const auto& interfaces = this->config->getInterfaces();
    std::cout << interfaces.size() << " of data " << std::endl;
    for (const auto& cfg : interfaces)
    {
        if (cfg.interfaceType == "Serial")
            setupSerialPort(cfg);
    }
}

void DataGrabbSerial::interfacesStart
()
{
    std::cout << "[Serial] Starting async reads...\n";

    for (auto& [name, iface] : serialInterfaces)
    {
        std::cout << "[Serial] Starting read on: " << name << std::endl;
        startAsyncRead(name);
    }

    std::cout << "[Serial] Running io_context...\n";
    io.run();
    std::cout << "[Serial] io_context finished.\n";
}

void DataGrabbSerial::interfacesStop
()
{
    std::cout << "[Serial] Stopping io_context...\n";
    io.stop();

    for (auto& [name, iface] : serialInterfaces) {
        if (iface.port && iface.port->is_open()) {
            try {
                iface.port->cancel(); // przerwij oczekujące operacje
                iface.port->close();
                std::cout << "Closed serial port: " << name << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Error closing port " << name << ": " << e.what() << std::endl;
            }
        }
    }
}

void DataGrabbSerial::interfacesReset
()
{
    std::cout << "[Serial] Restarting io_context...\n";
    io.restart();
}

void DataGrabbSerial::setupSerialPort
(
    const InterfaceConfig& cfg
)
{
    try {
        SerialInterface iface;
        iface.port = std::make_unique<serial_port>(io, cfg.phy_name);

        iface.port->set_option(serial_port_base::baud_rate(cfg.baud));
        iface.port->set_option(serial_port_base::character_size(cfg.characterSize));

        if (cfg.parity == "none")
            iface.port->set_option(serial_port_base::parity(serial_port_base::parity::none));
        else if (cfg.parity == "odd")
            iface.port->set_option(serial_port_base::parity(serial_port_base::parity::odd));
        else if (cfg.parity == "even")
            iface.port->set_option(serial_port_base::parity(serial_port_base::parity::even));

        if (cfg.stopBits == "one")
            iface.port->set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
        else if (cfg.stopBits == "onepointfive")
            iface.port->set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::onepointfive));
        else if (cfg.stopBits == "two")
            iface.port->set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::two));

        if (cfg.flowControl == "none")
            iface.port->set_option(serial_port_base::flow_control(serial_port_base::flow_control::none));
        else if (cfg.flowControl == "software")
            iface.port->set_option(serial_port_base::flow_control(serial_port_base::flow_control::software));
        else if (cfg.flowControl == "hardware")
            iface.port->set_option(serial_port_base::flow_control(serial_port_base::flow_control::hardware));

        serialInterfaces.emplace(cfg.phy_name, std::move(iface));
        std::cout << "Initialized serial port: " << cfg.phy_name << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error initializing port " << cfg.phy_name << ": " << e.what() << std::endl;
    }
}

void DataGrabbSerial::startAsyncRead
(
    const std::string& name
)
{
    auto& iface = serialInterfaces.at(name);
    async_read_until(*iface.port, iface.buffer, '\n',
        [this, name](const boost::system::error_code& ec, std::size_t bytes_transferred) {
            handleRead(name, ec, bytes_transferred);
        });
}

void DataGrabbSerial::handleRead
(
    const std::string& phy_name, 
    const boost::system::error_code& ec, 
    std::size_t
)
{
    if (!ec)
    {
        auto& iface = serialInterfaces.at(phy_name);
        std::istream is(&iface.buffer);
        std::string line;
        std::getline(is, line);

        std::cout << "[RX][" << phy_name << "] " << line << std::endl;

        auto disps = this->config->getDisplaysConnectedToPhyName(phy_name);
        for(const auto& disp_name : disps)
            this->collector->buffer_Grab().push(disp_name,line);

        startAsyncRead(phy_name); // continue async read
    }
    else
    {
        std::cerr << "Read error on " << phy_name << ": " << ec.message() << std::endl;
    }
}