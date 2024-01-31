# SPDX-FileCopyrightText: 2023 Carl Schwan <carl@carlschwan.eu>
# SPDX-License-Identifier: BSD-2-Clause

add_executable(optiimage
    main.cpp
    imagemodel.cpp
    imagemodel.h
    optimizer.cpp
    optimizer.h
    setupcheck.cpp
    setupcheck.h
)

qt_add_qml_module(optiimage URI org.kde.optiimage NO_RESOURCE_TARGET_PATH
    QML_FILES
        contents/ui/main.qml
        contents/ui/OptimizePage.qml
        contents/ui/SettingsPage.qml
)

target_include_directories(optiimage PRIVATE ${CMAKE_BINARY_DIR})
target_link_libraries(optiimage PRIVATE
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
    QCoro::Core
    QCoro::Qml
)

kconfig_add_kcfg_files(optiimage GENERATE_MOC config.kcfgc)
install(TARGETS optiimage ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
