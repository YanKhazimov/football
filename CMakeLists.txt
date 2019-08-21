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
    featuredstats.h
    updater.h
)

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
    featuredstats.cpp
    updater.cpp
)

set (3RDPARTY
    qt_zip/zconf.h
    qt_zip/zlib.h
    qt_zip/zipwriter.h
    qt_zip/zipreader.h
    qt_zip/zip.cpp)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)
find_library(ZLIB NAME zlibwapi.lib HINTS ${CMAKE_SOURCE_DIR}/qt_zip)

add_executable(${PROJECT_NAME} WIN32 "main.cpp" "qml.qrc" ${SRC} ${3RDPARTY})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick ${ZLIB})
