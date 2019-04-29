cmake_minimum_required(VERSION 3.13)
project(KillTheDevilMUlti)

set(CMAKE_CXX_STANDARD 17)

add_executable(KillTheDevilMUlti main.cpp configuration/config.h
        server/Network/server.h server/Network/server.cpp server/Manager/Manager.h server/Manager/Manager.cpp server/Manager/Utils.h  server/Network/client_handler.cpp server/Network/client_handler.h
        server/Engine/GameObject.cpp server/Engine/GameObject.h server/GameEntities/Player.cpp server/GameEntities/Player.h server server/GameEntities/Bullet.cpp server/GameEntities/Bullet.h
        client/Manager/Manager.cpp client/Manager/Manager.h client/Render/SpriteDrawer.cpp client/Render/SpriteDrawer.h
        client/Network/Client.cpp client/Network/Client.h server/Engine/Property.cpp server/Engine/Property.h client/Manager/KeyboardInterface.cpp client/Manager/KeyboardInterface.h)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED COMPONENTS network audio graphics window system)
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})