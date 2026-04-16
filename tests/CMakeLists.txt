cmake_minimum_required(VERSION 3.20)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Test)

add_executable(GameTests
    unit/main.cpp
    ../../backend/Board.cpp
    ../../backend/Game.cpp
)

target_include_directories(GameTests PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/backend
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tests/unit
)

target_link_libraries(GameTests PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Test
)

target_compile_options(GameTests PRIVATE
    -Wall
    -Wextra
)

add_test(NAME GameTests COMMAND GameTests)
