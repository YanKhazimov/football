cmake_minimum_required(VERSION 2.8.12)

project(untitled LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set (HEADERS query.h playersmodel.h playerstatsmodel.h gamesmodel.h player.h)
set (SRC query.cpp playersmodel.cpp playerstatsmodel.cpp gamesmodel.cpp player.cpp)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_executable(${PROJECT_NAME} "main.cpp" "qml.qrc" ${SRC})

#target_include_directories(untitled PUBLIC include)


target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick)
