// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

import QtQuick
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.delegates as Delegates
import QtQuick.Controls as QQC2
import QtQuick.Layouts
import Qt.labs.platform
import org.kde.optiimage
import org.kde.coreaddons

Kirigami.ScrollablePage {
    id: root

    title: i18n("Optimize your images")

    actions: [
        Kirigami.Action {
            id: addImages
            icon.name: "list-add"
            text: i18n("Add images")
            onTriggered: fileDialog.open()
        },
        Kirigami.Action {
            icon.name: "settings-configure"
            text: i18n("Settings")
            onTriggered: pageStack.layers.push(settings);

        }
    ]

    Component {
        id: settings
        SettingsPage {}
    }

    FileDialog {
        id: fileDialog
        folder: StandardPaths.writableLocation(StandardPaths.PicturesLocation)
        fileMode: FileDialog.OpenFiles
        onAccepted: imageModel.addImages(files)
        nameFilters: [i18n("Supported images files (*.jpg *.jpeg *.png, *.svg, *.webp)")]
    }

    ListView {
        id: imageView

        currentIndex: -1

        model: ImageModel {
            id: imageModel
        }

        delegate: Delegates.RoundedItemDelegate {
            id: imageDelegate

            required property int index
            required property string displayName
            required property string filename
            required property int size
            required property int newSize
            required property bool alreadyOptimized
            required property bool processed

            readonly property string subtitle: if (alreadyOptimized) {
                return i18nc("Size information with no change", "%1 (No change)", Format.formatByteSize(size));
            } else if (newSize != -1) {
                return i18nc("Size information", "%1 -> %2 (%3% decrease)", Format.formatByteSize(size), Format.formatByteSize(newSize), ((size - newSize) / size).toFixed(2) * 100);
            } else {
                return Format.formatByteSize(size);
            }

            text: displayName
            icon {
                source: filename
                height: Kirigami.Units.iconSizes.large
                width: Kirigami.Units.iconSizes.large
            }

            contentItem: RowLayout {
                spacing: Kirigami.Units.smallSpacing

                Delegates.SubtitleContentItem {
                    subtitle: imageDelegate.subtitle
                    itemDelegate: imageDelegate
                }

                Kirigami.Icon {
                    Layout.preferredWidth: Kirigami.Units.iconSizes.medium
                    Layout.preferredHeight: Kirigami.Units.iconSizes.medium
                    source: 'checkbox-symbolic'
                    visible: imageDelegate.processed
                }
            }

            background: null
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
}
