cmake_minimum_required(VERSION 3.10)
project(MasterKong2D)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-g -Wno-write-strings -Wno-return-type")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

set(SRC_FILES
    ParticleHandler.cpp
    about.cpp
    game.cpp
    gameover.cpp
    gintro.cpp
    grandma.cpp
    hero.cpp
    intro.cpp
    mastermain.cpp
    mxfont.cpp
    options.cpp
    pixelbuffer.cpp
    soundhandle.cpp
    start.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
