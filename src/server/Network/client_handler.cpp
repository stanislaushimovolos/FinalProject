#include "client_handler.h"

namespace ser
{

Handler::Handler(sf::TcpSocket *sock) :
    _socket(sock),
    _info(sock)
{}


Handler::Handler() :
    _socket(new sf::TcpSocket),
    _info(_socket)
{}


Handler::Handler(Handler &&other_client) noexcept :
    _socket(other_client._socket),
    _info(_socket)
{
    other_client._socket = nullptr;
}


Handler &Handler::operator=(Handler &&other_client) noexcept
{
    this->_socket = other_client._socket;
    this->_info = other_client._info;

    other_client._socket = nullptr;
    return *this;
}


Handler::~Handler()
{
    std::cout << "Client was deleted" << std::endl;
    delete _socket;
}


sf::TcpSocket *Handler::get_socket_ptr() const
{
    return _socket;
}


std::pair<uint32_t, uint32_t> Handler::info() const
{
    return _info.info();
}

}