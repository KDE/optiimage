// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

import QtQuick 2.15
import org.kde.kirigami 2.20 as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1
import org.kde.optiimage
import org.kde.coreaddons

Kirigami.ApplicationWindow {
    id: root

    width: 600

    globalDrawer: Kirigami.GlobalDrawer {
        isMenu: true
        actions: [
            Kirigami.Action {
                icon.name: "help-about-symbolic"
                text: i18n("About OptiImage")
                onTriggered: pageStack.pushDialogLayer(aboutOptiImage, {}, {
                })
            },
            Kirigami.Action {
                icon.name: "kde"
                text: i18n("About KDE")
                onTriggered: pageStack.pushDialogLayer(aboutKde)
            },
            Kirigami.Action {
                icon.name: "settings-configure"
                text: i18n("Settings")
                onTriggered: pageStack.pushDialogLayer('qrc:/SettingsPage.qml', {}, { title: i18n("Settings") })
            }
        ]
    }

    pageStack.initialPage: OptimizePage {}

    Component {
        id: aboutKde
        FormCard.AboutKDE {}
    }

    Component {
        id: aboutOptiImage
        FormCard.AboutPage {
            aboutData: AboutData
        }
    }
}
