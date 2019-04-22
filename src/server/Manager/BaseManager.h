#pragma once

#include "../Network/client_handler.h"
#include "../../configuration/config.h"
#include "Utils.h"

#include <unordered_map>
#include <list>

class BaseManager
{

 private:
    std::unordered_map<ser::Info, Player, ClientHasher, EqualClients>
        players;
 public:
    std::vector<std::pair<sf::Uint16,
                          ser::Info >> process_packets(std::vector<ser::Packet> &received_data) const;

    int update_state(std::vector<ser::Packet> &received_data);

    std::vector<ser::Packet> get_players_states();

    int add_players(const std::list<ser::Handler> &clients);

    BaseManager() = default;
};
