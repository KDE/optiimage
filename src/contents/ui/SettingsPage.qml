// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.0-or-later

import QtQuick 2.15
import org.kde.kirigami 2.18 as Kirigami
import QtQuick.Layouts 1.15
import org.kde.kirigamiaddons.labs.mobileform 0.1 as MobileForm
import org.kde.optiimage 1.0

Kirigami.ScrollablePage {
    id: root

    title: i18nc("@title:window", "Settings")

    leftPadding: 0
    rightPadding: 0

    ColumnLayout {
        MobileForm.FormCard {
            Layout.topMargin: Kirigami.Units.largeSpacing
            Layout.fillWidth: true
            contentItem: ColumnLayout {
                MobileForm.FormCardHeader {
                    title: i18n("General")
                }

                MobileForm.FormSwitchDelegate {
                    id: safeModeCheck
                    text: i18n("Safe mode")
                    description: i18n("Save the compressed image in a new file")
                    checked: Config.safeMode
                    onCheckedChanged: {
                        Config.safeMode = checked;
                        Config.save();
                    }
                }

                MobileForm.FormDelegateSeparator {
                    above: safeModeCheck
                }

                MobileForm.FormTextFieldDelegate {
                    enabled: safeModeCheck.checked
                    label: i18n("Suffix to append at the end of new file")
                    text: Config.suffix
                    onTextChanged: {
                        Config.suffix = text;
                        Config.save();
                    }
                }
            }
        }

        MobileForm.FormCard {
            Layout.topMargin: Kirigami.Units.largeSpacing
            Layout.fillWidth: true
            contentItem: ColumnLayout {
                spacing: 0
                Component {
                    id: aboutPage
                    MobileForm.AboutPage {
                        aboutData: About
                    }
                }
                Component {
                    id: aboutKDE
                    MobileForm.AboutKDE {}
                }

                MobileForm.FormButtonDelegate {
                    text: i18n("About OptiImage")
                    icon.name: "org.kde.optiimage"
                    onClicked: applicationWindow().pageStack.layers.push(aboutPage)
                }

                MobileForm.FormDelegateSeparator {}

                MobileForm.FormButtonDelegate {
                    text: i18n("About KDE")
                    icon.name: "kde"
                    onClicked: applicationWindow().pageStack.layers.push(aboutKDE)
                }
            }
        }
    }
}