// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

import QtQuick 2.15
import org.kde.kirigami 2.14 as Kirigami
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1
import org.kde.optiimage 1.0
import org.kde.kcoreaddons 1.0 as KCA

Kirigami.ApplicationWindow {
    id: root

    title: i18n("OptiImage")

    globalDrawer: Kirigami.GlobalDrawer {
        title: i18n("OptiImage")
        titleIcon: "applications-graphics"
        actions: [
            Kirigami.Action {
                text: i18n("View")
                icon.name: "view-list-icons"
                Kirigami.Action {
                    text: i18n("View Action 1")
                    onTriggered: showPassiveNotification(i18n("View Action 1 clicked"))
                }
                Kirigami.Action {
                    text: i18n("View Action 2")
                    onTriggered: showPassiveNotification(i18n("View Action 2 clicked"))
                }
            },
            Kirigami.Action {
                text: i18n("Action 1")
                onTriggered: showPassiveNotification(i18n("Action 1 clicked"))
            },
            Kirigami.Action {
                text: i18n("Action 2")
                onTriggered: showPassiveNotification(i18n("Action 2 clicked"))
            }
        ]
    }

    contextDrawer: Kirigami.ContextDrawer {
        id: contextDrawer
    }

    pageStack.initialPage: mainPageComponent
    

    Component {
        id: mainPageComponent

        Kirigami.ScrollablePage {
            title: i18n("Optimize your images")
    
            FileDialog {
                id: fileDialog
                folder: StandardPaths.writableLocation(StandardPaths.PicturesLocation)
                fileMode: FileDialog.OpenFiles
                onAccepted: imageModel.addImages(files)
                nameFilters: [i18n("Supported images files (*.jpg *.jpeg *.png)")]
            }

            actions {
                main: Kirigami.Action {
                    id: addImages
                    icon.name: "list-add"
                    text: i18n("Add images")
                    onTriggered: fileDialog.open()
                }
            }
            
            ListView {
                id: imageView
                model: ImageModel {
                    id: imageModel
                }
                
                delegate: Kirigami.BasicListItem {
                    label: display
                    icon: filename
                    subtitle: KCA.Format.formatByteSize(size) + (newSize ?  " → " + KCA.Format.formatByteSize(newSize) + i18n(" (%1% decrease)", ((size - newSize) / size).toFixed(2) * 100) : "")
                    trailing: Controls.CheckBox {
                        visible: processed
                        checkState: Qt.Checked
                        enabled: false
                    }
                }
                
                Kirigami.PlaceholderMessage {
                    visible: imageView.count === 0
                    text: i18n("No Images to optimize")
                    helpfulAction: addImages
                    anchors.centerIn: parent
                }
            }
            
            footer: Controls.ToolBar {
                visible: imageView.count > 0
                contentItem: RowLayout {
                    Controls.Button {
                        Layout.alignment: Qt.AlignRight
                        icon.name: "media-playback-start"
                        text: i18n("Optimize")
                        onClicked: imageModel.optimize()
                    }
                }
            }
        }
    }
}
