cmake_minimum_required(VERSION 3.16)
project(cpp-actor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

include_directories(include)

set(ACTOR_SOURCES
    src/actor.cpp
    src/actor_system.cpp
    src/message.cpp
)

set(ACTOR_HEADERS
    include/actor.h
    include/actor_system.h
    include/message.h
)

add_library(actor_lib ${ACTOR_SOURCES} ${ACTOR_HEADERS})
target_link_libraries(actor_lib Threads::Threads)

add_executable(cpp-actor src/main.cpp)
target_link_libraries(cpp-actor actor_lib)

add_subdirectory(examples)