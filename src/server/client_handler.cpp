#include "client_handler.h"


ClientHandler::ClientHandler(sf::TcpSocket *sock) :
    _socket(sock),
    _info(sock)
{}


ClientHandler::ClientHandler() :
    _socket(new sf::TcpSocket),
    _info(_socket)
{}


ClientHandler::ClientHandler(ClientHandler &&other_client) noexcept :
    _socket(other_client._socket),
    _info(_socket)
{
    other_client._socket = nullptr;
}


ClientHandler &ClientHandler::operator=(ClientHandler &&other_client) noexcept
{
    this->_socket = other_client._socket;
    this->_info = other_client._info;

    other_client._socket = nullptr;
    return *this;
}


ClientHandler::~ClientHandler()
{
    std::cout << "Client was deleted" << std::endl;
    delete _socket;
}


sf::TcpSocket *ClientHandler::get_socket_ptr()
{
    return _socket;
}


std::pair<std::string, uint32_t> ClientHandler::info()
{
    return _info.info();
}
