cmake_minimum_required(VERSION 3.13)
project(KillTheDevilMUlti)

set(CMAKE_CXX_STANDARD 17)

add_executable(KillTheDevilMUlti main.cpp
        server/Network/Server.h server/Network/Server.cpp server/Managers/GameManager.h server/Managers/GameManager.cpp server/Managers/Utils.h server/Network/ClientHandler.cpp server/Network/ClientHandler.h
        server/Engine/GameObject.cpp server/Engine/GameObject.h server/GameEntities/Player.cpp server/GameEntities/Player.h server server/GameEntities/Bullet.cpp server/GameEntities/Bullet.h
        client/Manager/ClientManager.cpp client/Manager/ClientManager.h client/Render/SpriteDrawer.cpp client/Render/SpriteDrawer.h
        client/Network/Client.cpp client/Network/Client.h server/Engine/Properties.cpp server/Engine/Properties.h client/Manager/KeyboardInterface.cpp client/Manager/KeyboardInterface.h
        server/Engine/RectangleCollider.cpp server/Engine/RectangleCollider.h configuration/network.h configuration/gameplay.h configuration/render.h
        server/GameEntities/Blast.cpp server/GameEntities/Blast.h server/GameEntities/MovingPlatform.cpp server/GameEntities/MovingPlatform.h
        map/tinyxml/tinystr.cpp map/tinyxml/tinystr.h map/tinyxml/tinyxml.cpp map/tinyxml/tinyxml.h map/tinyxml/tinyxmlerror.cpp map/tinyxml/tinyxmlparser.cpp map/Level.h map/Level.cpp map/Level.h
        server/Managers/LevelManager.cpp server/Managers/LevelManager.h configuration/map.h server/GameEntities/SolidBlock.cpp server/GameEntities/SolidBlock.h
        client/Menu/Menu.cpp client/Menu/Menu.h)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package (Threads)
find_package(X11 REQUIRED)
find_package(SFML 2 REQUIRED COMPONENTS network audio graphics window system)
target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})