# SPDX-FileCopyrightText: 2023 Carl Schwan <carl@carlschwan.eu>
# SPDX-License-Identifier: BSD-2-Clause

add_executable(optiimage
    main.cpp
    imagemodel.cpp
    optimizer.cpp
    format.cpp
    resources.qrc
)

target_link_libraries(optiimage
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
    Qt::QuickControls2
    Qt::Svg
    KF${QT_MAJOR_VERSION}::I18n
    KF${QT_MAJOR_VERSION}::CoreAddons
    KF${QT_MAJOR_VERSION}::ConfigGui
    QCoro::Core
    QCoro::Qml
)

kconfig_add_kcfg_files(optiimage GENERATE_MOC config.kcfgc)
install(TARGETS optiimage ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
