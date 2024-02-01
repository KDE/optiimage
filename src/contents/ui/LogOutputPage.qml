// SPDX-FileCopyrightText: 2024 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

import QtQuick
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.delegates as Delegates
import org.kde.kirigamiaddons.components as Components
import QtQuick.Controls as QQC2
import QtQuick.Layouts
import Qt.labs.platform
import org.kde.optiimage
import org.kde.coreaddons

Kirigami.ScrollablePage {
    title: i18nc("@title", "Console output")
    ListView {
        id: logView

        currentIndex: -1

        model: ConsoleLog

        delegate: Delegates.RoundedItemDelegate {
            id: logOutputDelegate

            required property int index
            required property string output
            required property string imagePath

            background: null
            contentItem: ColumnLayout {
                width: parent.width
                spacing: Kirigami.Units.smallSpacing

                Kirigami.Separator {
                    Layout.fillWidth: true
                    visible: logOutputDelegate.index !== 0
                }

                Kirigami.Heading {
                    level: 3
                    text: logOutputDelegate.imagePath
                    elide: Text.ElideMiddle
                    Layout.fillWidth: true
                }

                QQC2.Label {
                    text: logOutputDelegate.output
                    wrapMode: TextEdit.WordWrap
                    Layout.fillWidth: true
                }
            }
        }

        Kirigami.PlaceholderMessage {
            visible: logView.count === 0
            text: i18n("No log output")
            width: parent.width - Kirigami.Units.gridUnit * 4
            anchors.centerIn: parent
        }
    }
}
