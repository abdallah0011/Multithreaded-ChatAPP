cmake_minimum_required(VERSION 3.22)
project(MultithrededChatAPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS "-g") # symbol table for debugging

# add_subdirectory(src)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/lib/includes/)
    
add_executable(TCPserver tests/MultithreadedChatServer.cpp)
target_link_libraries(TCPserver PUBLIC TCPlib)

add_executable(TCPclient tests/MultithreadedChatClient.cpp)
target_link_libraries(TCPclient PUBLIC TCPlib)

install(TARGETS TCPserver DESTINATION ${CMAKE_SOURCE_DIR}/run)
install(TARGETS TCPclient DESTINATION ${CMAKE_SOURCE_DIR}/run)
