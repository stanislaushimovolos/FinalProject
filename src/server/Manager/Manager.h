#pragma once

#include "../Network/client_handler.h"
#include "../../configuration/config.h"
#include "Utils.h"

#include <unordered_map>
#include <list>

class Manager
{

 private:
    std::unordered_map<Info, Player, ClientHasher, EqualClients>
        players;
 public:
    std::vector<std::pair<sf::Uint16,
                          Info >> process_packets(std::vector<Packet> &received_data) const;

    int update_state(std::vector<Packet> &received_data);

    std::vector<Packet> get_players_states();

    int add_players(const std::list<Handler> &clients);

    Manager() = default;
};
