cmake_minimum_required(VERSION 3.22)
project(MultithrededChatAPPLibrary)

file(GLOB SOURCES
    src/*.cpp)

file(GLOB HEADERS
    includes/*.hpp)

# set(SOURCES
#     src/socket.cpp
#     src/client.cpp
#     src/server.cpp
#     src/user.cpp)

# set(HEADERS
#     includes/socket.hpp
#     includes/client.hpp
#     includes/server.hpp
#     includes/user.hpp)

include_directories(includes)

add_library(TCPlib STATIC ${SOURCES} ${HEADERS})

