cmake_minimum_required(VERSION 3.13)
project(KillTheDevilMUlti)

set(CMAKE_CXX_STANDARD 17)

add_executable(KillTheDevilMUlti main.cpp server/Network/server.h server/Network/server.cpp client/client.cpp
        client/client.h server/Network/client_handler.cpp server/Network/client_handler.h server/Manager/BaseManager.h
        configuration/config.h server/Engine/GameObject.cpp server/Engine/GameObject.h server/Manager/BaseManager.cpp server/Manager/Utils.h server/GameEntities/Player.cpp server/GameEntities/Player.h)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED COMPONENTS network audio graphics window system)
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})