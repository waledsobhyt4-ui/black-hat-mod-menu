cmake_minimum_required(VERSION 3.21)
project(BlackHatModMenu)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} SHARED)

target_sources(${PROJECT_NAME} PRIVATE
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Geode::Geode)
