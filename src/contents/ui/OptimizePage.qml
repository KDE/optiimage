// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

import QtQuick 2.15
import org.kde.kirigami 2.20 as Kirigami
import QtQuick.Controls 2.15 as QQC2
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1
import org.kde.optiimage 1.0

Kirigami.ScrollablePage {
    id: root

    title: i18n("Optimize your images")

    actions.main: Kirigami.Action {
        id: addImages
        icon.name: "list-add"
        text: i18n("Add images")
        onTriggered: fileDialog.open()
    }

    FileDialog {
        id: fileDialog
        folder: StandardPaths.writableLocation(StandardPaths.PicturesLocation)
        fileMode: FileDialog.OpenFiles
        onAccepted: imageModel.addImages(files)
        nameFilters: [i18n("Supported images files (*.jpg *.jpeg *.png)")]
    }

    ListView {
        id: imageView

        model: ImageModel {
            id: imageModel
        }

        delegate: Kirigami.BasicListItem {
            label: display
            icon: filename
            subtitle: Format.formatByteSize(size) + (newSize ?  " → " + Format.formatByteSize(newSize) + i18n(" (%1% decrease)", ((size - newSize) / size).toFixed(2) * 100) : "")
            trailing: QQC2.CheckBox {
                visible: processed
                checkState: Qt.Checked
                enabled: false
            }
        }

        Kirigami.PlaceholderMessage {
            visible: imageView.count === 0
            text: i18n("No Images to optimize")
            helpfulAction: addImages
            width: parent.width - Kirigami.Units.gridUnit * 4
            icon.name: "org.kde.optiimage"
            anchors.centerIn: parent
        }
    }

    footer: QQC2.ToolBar {
        visible: imageView.count > 0
        height: visible ? implicitHeight : 0
        contentItem: RowLayout {
            QQC2.Button {
                Layout.alignment: Qt.AlignRight
                icon.name: "media-playback-start"
                text: i18n("Optimize")
                onClicked: imageModel.optimize()
            }
        }
    }
}
