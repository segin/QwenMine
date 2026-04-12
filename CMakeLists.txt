cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

project(QwenMine LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(QwenMine
    src/main.cpp
    backend/Board.cpp
    backend/Cell.cpp
    backend/Game.cpp
    frontend/BoardView.cpp
    frontend/GameWindow.cpp
)

target_include_directories(QwenMine PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/backend
    ${CMAKE_SOURCE_DIR}/frontend
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(QwenMine PRIVATE
    Qt6::Core
    Qt6::Widgets
)

target_compile_options(QwenMine PRIVATE
    -Wall
    -Wextra
    -Werror
)

install(TARGETS QwenMine BUNDLE DESTINATION .)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
