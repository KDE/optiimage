# SPDX-FileCopyrightText: 2023 Carl Schwan <carl@carlschwan.eu>
# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.16)

set(QT_MIN_VERSION 5.15)
set(KF_MIN_VERSION 5.104)
set(RELEASE_SERVICE_VERSION "0.1.0") # TODO move to release service
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(OptiImage VERSION ${RELEASE_SERVICE_VERSION})

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMPoQmTools)
include(KDEGitCommitHooks)
include(ECMCheckOutboundLicense)
include(KDEClangFormat)

ecm_setup_version(${RELEASE_SERVICE_VERSION}
    VARIABLE_PREFIX OPTIIMAGE
    VERSION_HEADER ${CMAKE_CURRENT_BINARY_DIR}/optiimage-version.h
)

find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} REQUIRED COMPONENTS Core Gui Qml QuickControls2 Svg)
find_package(KF${QT_MAJOR_VERSION} ${KF_MIN_VERSION} REQUIRED COMPONENTS Kirigami2 I18n CoreAddons Config)
find_package(QCoro${QT_MAJOR_VERSION} 0.4 COMPONENTS Core Qml REQUIRED)

qcoro_enable_coroutines()

add_subdirectory(src)

install(PROGRAMS org.kde.optiimage.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.optiimage.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(FILES org.kde.optiimage.svg DESTINATION ${KDE_INSTALL_FULL_ICONDIR}/hicolor/scalable/apps)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
