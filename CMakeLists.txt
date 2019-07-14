cmake_minimum_required(VERSION 2.8.12)

project(Razdevalka LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set (HEADERS
    playerbase.h
    featuredstatsmodel.h
    personalstatsmodel.h
    gamesmodel.h
    player.h
    globalstatsmodel.h
    teamsplitter.h
    dataroles.h
    playersortfilterproxymodel.h
    statpresenterproxymodel.h
    language.h
    config.h
    syncmanager.h
    award.h
    featuredstats.h)

set (SRC
    playerbase.cpp
    featuredstatsmodel.cpp
    personalstatsmodel.cpp
    gamesmodel.cpp
    player.cpp
    globalstatsmodel.cpp
    teamsplitter.cpp
    playersortfilterproxymodel.cpp
    statpresenterproxymodel.cpp
    language.cpp
    config.cpp
    syncmanager.cpp
    award.cpp
    featuredstats.cpp)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_executable(${PROJECT_NAME} WIN32 "main.cpp" "qml.qrc" ${SRC})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick)
