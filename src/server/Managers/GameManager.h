#pragma once

#include <list>
#include <unordered_map>

#include "../Engine/GameObject.h"
#include "../GameEntities/Blast.h"
#include "../GameEntities/Player.h"
#include "../GameEntities/Bullet.h"
#include "../GameEntities/MovingPlatform.h"
#include "../GameEntities/SolidBlock.h"

#include "../Network/ClientHandler.h"
#include "../../configuration/render.h"

#include "LevelManager.h"
#include "Utils.h"

namespace ser
{

class GameManager
{

 private:

    // Object with all information about level
    LevelManager _level;

    // Map of clients ip-port and their game objects
    std::unordered_map<ClientId, Player *, ClientHasher, EqualClients> _players;

    // Consists information about player's keyboard
    std::list<ClientInput> _players_states;

    // All game objects (players too)
    std::list<GameObject *> _objects;

 public:

    explicit GameManager(const std::string &file_name);

    void process_packets(std::vector<Packet> &received_data);

    void load_init_objects();

    void collect_garbage();

    int update_player_states(std::vector<ser::Packet> &received_data);

    int start_game(const std::list<ser::Handler> &clients);

    int update_game(sf::Time &&delta_t);

    int remove_disconnected_players(std::vector<ClientId> &dis_players);

    std::vector<uint64_t> get_players_ptr_id(const std::list<ser::Handler> &clients);

    sf::Packet create_current_state_packet();

    size_t count_players();

    ~GameManager();
};

}
