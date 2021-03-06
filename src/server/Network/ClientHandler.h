#pragma once

#include <SFML/Network.hpp>
#include <iostream>

namespace ser
{

class ClientId
{
 private:

    uint32_t _ip_address;;
    uint32_t _port;

 public:

    explicit ClientId(sf::TcpSocket *sock) :
        _ip_address(sock->getRemoteAddress().toInteger()),
        _port(sock->getRemotePort())
    {}

    explicit ClientId(const std::pair<uint32_t, uint32_t> &ip_port) :
        _ip_address(ip_port.first),
        _port(ip_port.second)
    {}

    std::pair<uint32_t, uint32_t> get_id() const
    {
        return std::make_pair(_ip_address, _port);
    }

};

class Packet
{
 private:

    sf::Packet _packet;
    ClientId _info;

 public:

    explicit Packet(ClientId &info) :
        _packet(),
        _info(info)
    {}

    explicit Packet(const std::pair<uint32_t, uint32_t> &ip_port) :
        _info(ip_port)
    {}

    sf::Packet &data()
    {
        return _packet;
    }

    std::pair<uint32_t, uint32_t> get_id()
    {
        return _info.get_id();
    }
};

class Handler
{
 private:
    sf::TcpSocket *_socket;
    ClientId _info;
 public:

    explicit Handler(sf::TcpSocket *sock);

    Handler();

    Handler(Handler &&other_client) noexcept;

    Handler &operator=(Handler &&other_client) noexcept;

    ~Handler();

    sf::TcpSocket *get_socket_ptr() const;

    std::pair<uint32_t, uint32_t> get_id() const;
};

}