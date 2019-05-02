cmake_minimum_required(VERSION 3.13)
project(KillTheDevilMUlti)

set(CMAKE_CXX_STANDARD 17)

add_executable(KillTheDevilMUlti main.cpp
        server/Network/Server.h server/Network/Server.cpp server/Manager/GameManager.h server/Manager/GameManager.cpp server/Manager/Utils.h server/Network/ClientHandler.cpp server/Network/ClientHandler.h
        server/Engine/GameObject.cpp server/Engine/GameObject.h server/GameEntities/Player.cpp server/GameEntities/Player.h server server/GameEntities/Bullet.cpp server/GameEntities/Bullet.h
        client/Manager/DrawManager.cpp client/Manager/DrawManager.h client/Render/SpriteDrawer.cpp client/Render/SpriteDrawer.h
        client/Network/Client.cpp client/Network/Client.h server/Engine/Properties.cpp server/Engine/Properties.h client/Manager/KeyboardInterface.cpp client/Manager/KeyboardInterface.h server/Engine/RectCollider.cpp server/Engine/RectCollider.h configuration/network.h configuration/gameplay.h configuration/render.h server/GameEntities/Blast.cpp server/GameEntities/Blast.h server/GameEntities/MovingPlatform.cpp server/GameEntities/MovingPlatform.h

        map/tinyxml/tinystr.cpp  map/tinyxml/tinystr.h  map/tinyxml/tinyxml.cpp  map/tinyxml/tinyxml.h  map/tinyxml/tinyxmlerror.cpp  map/tinyxml/tinyxmlparser.cpp map/level.h map/Level.cpp map/Level.h)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED COMPONENTS network audio graphics window system)
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})