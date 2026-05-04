cmake_minimum_required(VERSION 3.21)

project(ImagoRefMobile VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick QuickControls2 Qml Svg)

qt_standard_project_setup(REQUIRES 6.5)
qt_policy(SET QTP0004 NEW)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

qt_add_executable(ImagoRefMobile MANUAL_FINALIZATION
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/managers/ThemeManager.h
    ${SRC_DIR}/managers/ThemeManager.cpp
    ${SRC_DIR}/managers/AppState.h
    ${SRC_DIR}/managers/AppState.cpp
)

target_include_directories(ImagoRefMobile PRIVATE
    ${SRC_DIR}/managers
)

qt_add_qml_module(ImagoRefMobile
    URI ImagoRefMobile
    VERSION 1.0
    QML_FILES
        src/qml/Main.qml
        src/qml/pages/HomePage.qml
        src/qml/components/BackgroundDecor.qml
        src/qml/components/AccountChip.qml
        src/qml/components/BoardCard.qml
)

target_link_libraries(ImagoRefMobile PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Qml
    Qt6::Svg
)

add_custom_command(TARGET ImagoRefMobile POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/res/themes"
    "$<TARGET_FILE_DIR:ImagoRefMobile>/themes"
)

set_target_properties(ImagoRefMobile PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(ImagoRefMobile)
