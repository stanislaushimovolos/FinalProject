#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <list>
#include <unistd.h>

const uint16_t default_port = 8080;

class Server
{
 public:

  explicit Server(uint16_t port = default_port);

  int handle();

  ~Server();

 private:
  sf::TcpListener _listener;
  sf::IpAddress _ip_address;
  sf::SocketSelector _selector;

  std::list<sf::TcpSocket *> _clients;
  uint16_t _port;

  int add_new_client(sf::TcpSocket *client);

  std::vector<sf::Packet> receive_data();

  int process_received_data(const std::vector<sf::Packet> &received_data);
};