#pragma once

#include <list>
#include <unordered_map>

#include "../Engine/GameObject.h"
#include "../GameEntities/Player.h"
#include "../GameEntities/Bullet.h"
#include "../Network/ClientHandler.h"
#include "../../configuration/render.h"
#include "Utils.h"

namespace ser
{

class Manager
{

 private:

    std::unordered_map<ClientId, Player *, ClientHasher, EqualClients> _players;
    std::vector<ClientState> _players_states;
    std::list<GameObject *> _objects;

 public:

    void process_packets(std::vector<Packet> &received_data);

    int update_player_states(std::vector<ser::Packet> &received_data);

    int add_players(const std::list<ser::Handler> &clients);

    int update_environment(sf::Time &&delta_t);

    sf::Packet create_current_state_packet();

    Manager() = default;

    ~Manager();
};

}
