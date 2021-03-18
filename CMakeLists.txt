cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(raskble)
include(GNUInstallDirs)

################################
# set lib version here

set(GENERIC_LIB_VERSION "1.0.0")
set(GENERIC_LIB_SOVERSION "1")

################################
# Add targets
# By Default shared library is being built
# To build static libs also - Do cmake . -DBUILD_STATIC_LIBS:BOOL=ON
# User can choose not to build shared library by using cmake -DBUILD_SHARED_LIBS:BOOL=OFF
# To build only static libs use cmake . -DBUILD_SHARED_LIBS:BOOL=OFF -DBUILD_STATIC_LIBS:BOOL=ON

option(BUILD_SHARED_LIBS "build as shared library" ON)

set(CMAKE_DEBUG_POSTFIX "d")

add_library(${PROJECT_NAME} 
    rask-ble.h
    rask-ble.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_DEFINITIONS "RASKBLE_EXPORT"
	VERSION "${GENERIC_LIB_VERSION}"
	SOVERSION "${GENERIC_LIB_SOVERSION}"
)

target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Debug>:RASKBLE_DEBUG>)

target_include_directories(${PROJECT_NAME} 
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# export targets for find_package config mode
export(TARGETS ${PROJECT_NAME}
    FILE ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}Targets.cmake
)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${CMAKE_PROJECT_NAME}Targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT ${CMAKE_PROJECT_NAME}Targets NAMESPACE raslble::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}
)
