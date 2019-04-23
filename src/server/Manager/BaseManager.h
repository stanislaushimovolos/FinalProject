#pragma once

#include "../../configuration/config.h"
#include "../Network/client_handler.h"
#include "Utils.h"
#include "../GameEntities/Player.h"

#include <unordered_map>
#include <list>

namespace ser
{

class BaseManager
{

 private:
    std::unordered_map<ser::Info, ser::Player, ClientHasher, EqualClients>
        players;
 public:
    std::vector<std::pair<sf::Uint16,
                          ser::Info >> process_packets(std::vector<ser::Packet> &received_data) const;

    sf::Packet create_current_state_packet();

    int update_state(std::vector<ser::Packet> &received_data);

    int add_players(const std::list<ser::Handler> &clients);

    BaseManager() = default;
};

}
