cmake_minimum_required(VERSION 3.22)

project(Application_File)

set(CMAKE_CXX_STANDARD 20)

set(QT_SOURCES src/qt/mainwindow.cpp src/qt/diskinfowindow.cpp src/qt/txtwindow.cpp src/qt/jsonwindow.cpp src/qt/xmlwindow.cpp src/qt/zipwindow.cpp)

set(SOURCES src/main.cpp ${QT_SOURCES})

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(AppFile ${SOURCES})
target_compile_options(AppFile PRIVATE -Wall -Werror -Wextra -Wreturn-type)
target_link_libraries(AppFile PRIVATE Qt6::Core Qt6::Widgets)