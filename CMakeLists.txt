cmake_minimum_required(VERSION 2.8.12)

project(untitled LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set (HEADERS
    playerbase.h
    featuredstatsmodel.h
    playerstatsmodel.h
    gamesmodel.h
    player.h
    globalstatsmodel.h
    teamsplitter.h
    dataroles.h
    playersortfilterproxymodel.h
    statpresenterproxymodel.h
    customextendablemodel.h
    featuredstats.h)

set (SRC
    playerbase.cpp
    featuredstatsmodel.cpp
    playerstatsmodel.cpp
    gamesmodel.cpp
    player.cpp
    globalstatsmodel.cpp
    teamsplitter.cpp
    playersortfilterproxymodel.cpp
    statpresenterproxymodel.cpp
    customextendablemodel.cpp
    featuredstats.cpp)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_executable(${PROJECT_NAME} "main.cpp" "qml.qrc" ${SRC})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick)
