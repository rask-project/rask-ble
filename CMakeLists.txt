cmake_minimum_required(VERSION 3.14)

project(RaskBle VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

set(libRaskBLE_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/raskble_plugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/raskble_plugin.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/characteristic.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/characteristic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/constants.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/controller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/controller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/device.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/service.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/resources/qmldir
)

set(QT_INSTALL_QMLDIR "$ENV{QTDIR}/qml")
set(plugin_PATH plugins/Rask/Ble)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick QuickControls2 Mqtt Bluetooth REQUIRED)

add_library(${PROJECT_NAME}
    SHARED
        ${libRaskBLE_SRCS}
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        AUTOMOC TRUE
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../${plugin_PATH}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Quick
        Qt${QT_VERSION_MAJOR}::QuickControls2
        Qt${QT_VERSION_MAJOR}::Bluetooth
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${QT_INSTALL_QMLDIR}/Rask/Ble)
install(FILES resources/qmldir DESTINATION ${QT_INSTALL_QMLDIR}/Rask/Ble)
