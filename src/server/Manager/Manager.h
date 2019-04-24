#pragma once

#include "../../configuration/config.h"
#include "../Network/client_handler.h"
#include "../GameEntities/Player.h"
#include "../Engine/GameObject.h"

#include "Utils.h"

#include <unordered_map>
#include <list>

namespace ser
{

class Manager
{

 private:
    std::unordered_map<Info, Player *, ClientHasher, EqualClients> _players;
    std::vector<GameObject *> _objects;


 public:
    std::vector<std::pair<sf::Uint16,
                          ser::Info >> process_packets(std::vector<ser::Packet> &received_data) const;

    sf::Packet create_current_state_packet();

    int update_player_states(std::vector<ser::Packet> &received_data);

    int add_players(const std::list<ser::Handler> &clients);

    int update_environment();

    Manager() = default;

    // Add Destructor for objects!!!!!!!!!!!!!!!!!!!!!!!!!
};

}
