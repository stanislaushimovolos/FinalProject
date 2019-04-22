cmake_minimum_required(VERSION 3.13)
project(KillTheDevilMUlti)

set(CMAKE_CXX_STANDARD 17)

add_executable(KillTheDevilMUlti main.cpp server/Network/server.h server/Network/server.cpp client/client.cpp
        client/client.h server/Network/client_handler.cpp server/Network/client_handler.h server/Manager/Manager.h
        configuration/config.h server/Engine/gameObject.cpp server/Engine/gameObject.h  server/Manager/Manager.cpp server/Manager/Utils.h)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED COMPONENTS network audio graphics window system)
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})