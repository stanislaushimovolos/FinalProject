#pragma once

#include <SFML/Network.hpp>
#include <iostream>

class ClientInfo
{
 public:
  std::string _ip_address;;
  uint32_t _port;

  explicit ClientInfo(sf::TcpSocket *sock) :
      _ip_address(sock->getRemoteAddress().toString()),
      _port(sock->getRemotePort())
  {}

};

class ClientHandler
{

 public:
  sf::TcpSocket *_socket;
  ClientInfo _info;

  explicit ClientHandler(sf::TcpSocket *sock) :
      _socket(sock),
      _info(sock)
  {}

  ClientHandler() :
      _socket(new sf::TcpSocket),
      _info(_socket)
  {}

  ClientHandler(ClientHandler &&other_client) noexcept:
      _socket(other_client._socket),
      _info(_socket)
  {
      other_client._socket = nullptr;
  }

  ClientHandler &operator=(ClientHandler &&other_client) noexcept
  {
      this->_socket = other_client._socket;
      this->_info = other_client._info;

      other_client._socket = nullptr;
      return *this;
  }

  ~ClientHandler()
  {
      std::cout << "Client was deleted" << std::endl;
      delete _socket;
  }
};

class ClientPacket
{
 private:
 public:
  sf::Packet packet;
  ClientInfo _info;

  explicit ClientPacket(ClientInfo &info) :
      packet(),
      _info(info)
  {}
};