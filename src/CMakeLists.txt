# SPDX-FileCopyrightText: 2023 Carl Schwan <carl@carlschwan.eu>
# SPDX-License-Identifier: BSD-2-Clause


qt_add_qml_module(optiimage_static
    STATIC
    URI org.kde.optiimage
    VERSION 1.0
    QML_FILES
        contents/ui/Main.qml
        contents/ui/OptimizePage.qml
        contents/ui/SettingsPage.qml
        contents/ui/LogOutputPage.qml
)

target_sources(optiimage_static PRIVATE
    consolelog.cpp
    consolelog.h
    imagemodel.cpp
    imagemodel.h
    optimizer.cpp
    optimizer.h
    setupcheck.cpp
    setupcheck.h
)

target_include_directories(optiimage_static PUBLIC ${CMAKE_BINARY_DIR})
target_link_libraries(optiimage_static PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::Qml
    Qt::Quick
    Qt::QuickControls2
    Qt::Svg
    Qt::Widgets
    KF6::I18n
    KF6::CoreAddons
    KF6::ConfigGui
    KF6::KIOCore
    KF6::KIOFileWidgets
    KF6::IconThemes
    QCoro::Core
    QCoro::Qml
)

kconfig_target_kcfg_file(optiimage_static FILE config.kcfg CLASS_NAME Config MUTATORS GENERATE_PROPERTIES GENERATE_MOC DEFAULT_VALUE_GETTERS PARENT_IN_CONSTRUCTOR QML_REGISTRATION SINGLETON)

add_executable(optiimage
    main.cpp
)
target_link_libraries(optiimage PUBLIC optiimage_static optiimage_staticplugin)

if (TARGET KF6::Crash)
    target_link_libraries(optiimage PUBLIC KF6::Crash)
endif()
install(TARGETS optiimage ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
