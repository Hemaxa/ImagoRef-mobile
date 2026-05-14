cmake_minimum_required(VERSION 3.21)

project(ImagoRefMobile VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Network Quick QuickControls2 Qml Svg)

qt_standard_project_setup(REQUIRES 6.5)
qt_policy(SET QTP0004 NEW)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(APP_ID com.imagoref.mobile)
set(APP_DISPLAY_NAME "ImagoRef Mobile")
set(APP_ICON_MACOS "${CMAKE_CURRENT_SOURCE_DIR}/res/app-icon/icon.icns")
set(IOS_ASSET_CATALOG "${CMAKE_CURRENT_SOURCE_DIR}/packs/ios/Assets.xcassets")

qt_add_executable(ImagoRefMobile MANUAL_FINALIZATION
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/managers/BoardsListModel.h
    ${SRC_DIR}/managers/BoardsListModel.cpp
    ${SRC_DIR}/managers/BoardItemsModel.h
    ${SRC_DIR}/managers/BoardItemsModel.cpp
    ${SRC_DIR}/managers/CacheManager.h
    ${SRC_DIR}/managers/CacheManager.cpp
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
        src/qml/pages/BoardPage.qml
            src/qml/components/BackgroundDecor.qml
            src/qml/components/AccountChip.qml
            src/qml/components/BoardCard.qml
            src/qml/components/CircularAvatar.qml
    )

target_link_libraries(ImagoRefMobile PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Network
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

if(APPLE)
    set_source_files_properties(${APP_ICON_MACOS} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set_source_files_properties(${IOS_ASSET_CATALOG} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    target_sources(ImagoRefMobile PRIVATE
        ${APP_ICON_MACOS}
        ${IOS_ASSET_CATALOG}
    )
endif()

set_target_properties(ImagoRefMobile PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER ${APP_ID}
    MACOSX_BUNDLE_BUNDLE_NAME ${APP_DISPLAY_NAME}
    MACOSX_BUNDLE_ICON_FILE icon.icns
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/packs/Info.plist.in"
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
    QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/packs/android"
    QT_ANDROID_VERSION_NAME ${PROJECT_VERSION}
    QT_ANDROID_VERSION_CODE 1
)

qt_finalize_executable(ImagoRefMobile)
