#pragma once

#include <list>
#include <unordered_map>

#include "../Engine/GameObject.h"
#include "../GameEntities/Blast.h"
#include "../GameEntities/Player.h"
#include "../GameEntities/Bullet.h"
#include "../GameEntities/MovingPlatform.h"
#include "../Network/ClientHandler.h"
#include "../../configuration/render.h"

#include "LevelManager.h"
#include "Utils.h"

namespace ser
{

class GameManager
{

 private:

    LevelManager &_level;

    std::unordered_map<ClientId, Player *, ClientHasher, EqualClients> _players;
    std::list<ClientState> _players_states;
    std::list<GameObject *> _objects;

 public:

    explicit GameManager(LevelManager &_level);

    void process_packets(std::vector<Packet> &received_data);

    void collect_garbage();

    int update_player_states(std::vector<ser::Packet> &received_data);

    int add_players(const std::list<ser::Handler> &clients);

    int update_environment(sf::Time &&delta_t);

    int remove_disconnected_players(std::vector<ClientId> &dis_players);

    std::vector<uint64_t> get_players_ptr_id(const std::list<ser::Handler> &clients);

    sf::Packet create_current_state_packet();

    ~GameManager();
};

}
