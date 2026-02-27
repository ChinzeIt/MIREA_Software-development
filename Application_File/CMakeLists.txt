cmake_minimum_required(VERSION 3.22)

project(Application_File)

set(CMAKE_CXX_STANDARD 20)

set(CORE_SOURCE src/core/STDCreateZIPService.cpp)

set(QT_SOURCES src/qt/mainwindow.cpp src/qt/diskinfowindow.cpp src/qt/txtwindow.cpp src/qt/jsonwindow.cpp src/qt/xmlwindow.cpp src/qt/zipwindow.cpp)

set(PATH_ADAPTER_QT src/adapter/qt)

set(ADAPTER_SOURCE_QT ${PATH_ADAPTER_QT}/qtDiskService.cpp 
${PATH_ADAPTER_QT}/qtCheckPathService.cpp 
${PATH_ADAPTER_QT}/qtCreateFileService.cpp 
${PATH_ADAPTER_QT}/qtReadFileService.cpp 
${PATH_ADAPTER_QT}/qtEditFileService.cpp 
${PATH_ADAPTER_QT}/qtRemoveFileService.cpp 
${PATH_ADAPTER_QT}/qtDeserialJsonXMLService.cpp)

set(SOURCES src/main.cpp ${CORE_SOURCE} ${QT_SOURCES} ${ADAPTER_SOURCE_QT})

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Xml)

add_executable(AppFile ${SOURCES})
target_compile_options(AppFile PRIVATE -Wall -Werror -Wextra -Wreturn-type)
target_link_libraries(AppFile PRIVATE Qt6::Core Qt6::Widgets Qt6::Xml)

find_program(VALGRIND_EXECUTABLE valgrind)
add_custom_target(
    valgrind
    COMMAND ${VALGRIND_EXECUTABLE}
        --tool=memcheck
        --leak-check=full
        --show-leak-kinds=all
        --track-origins=yes
        --log-file=valgrind.log
        $<TARGET_FILE:AppFile>
    DEPENDS AppFile
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    VERBATIM
)

find_program(CPPCHECK_EXECUTABLE cppcheck)

add_custom_target(
    cppcheck
    COMMAND ${CPPCHECK_EXECUTABLE}
        --enable=all
        --std=c++20
        --inconclusive
        --quiet
        --inline-suppr
        --suppress=missingIncludeSystem
        --suppress=unusedFunction
        ${CMAKE_SOURCE_DIR}/src
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    VERBATIM
)