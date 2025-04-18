# SPDX-FileCopyrightText: 2023 Carl Schwan <carl@carlschwan.eu>
# SPDX-License-Identifier: BSD-2-Clause
cmake_minimum_required(VERSION 3.20)

set(QT_MIN_VERSION 6.6)
set(KF_MIN_VERSION 6.8)
set(RELEASE_SERVICE_VERSION "0.1.0") # TODO move to release service
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(OptiImage VERSION ${RELEASE_SERVICE_VERSION})

include(FeatureSummary)

find_package(ECM ${KF_MIN_VERSION} NO_MODULE)
set_package_properties(ECM PROPERTIES
    TYPE REQUIRED
    URL "https://api.kde.org/ecm/"
    DESCRIPTION "extra cmake modules")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

find_package(Qt6 ${QT_MIN_VERSION} NO_MODULE  COMPONENTS Core Gui Qml QuickControls2 Svg Widgets)
set_package_properties(Qt6 PROPERTIES
    TYPE REQUIRED
    PURPOSE "Required application components"
)

find_package(KF6 ${KF_MIN_VERSION} COMPONENTS Kirigami I18n CoreAddons Config KIO IconThemes )
set_package_properties(KF6 PROPERTIES
    TYPE REQUIRED
    PURPOSE "Required application components"
)

find_package(QCoro6 0.4 COMPONENTS Core Qml)
set_package_properties(QCoro6 PROPERTIES
    TYPE REQUIRED
    PURPOSE "Required application components"
)

find_package(KF6Crash ${KF_MIN_VERSION})
set_package_properties(KF6Crash PROPERTIES
        TYPE OPTIONAL
        PURPOSE "For crash handling"
)

find_package(KF6KirigamiAddons 1.5.0)
set_package_properties(KF6KirigamiAddons PROPERTIES
    TYPE REQUIRED
    PURPOSE "Required application components and runtime dependency"
)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMPoQmTools)
include(KDEGitCommitHooks)
include(ECMCheckOutboundLicense)
include(KDEClangFormat)
include(ECMQmlModule)

ecm_setup_version(${RELEASE_SERVICE_VERSION}
    VARIABLE_PREFIX OPTIIMAGE
    VERSION_HEADER ${CMAKE_CURRENT_BINARY_DIR}/optiimage-version.h
)

qcoro_enable_coroutines()

add_subdirectory(src)

ki18n_install(po)

install(PROGRAMS org.kde.optiimage.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.optiimage.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(FILES org.kde.optiimage.svg DESTINATION ${KDE_INSTALL_FULL_ICONDIR}/hicolor/scalable/apps)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES src/*.cpp src/*.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
