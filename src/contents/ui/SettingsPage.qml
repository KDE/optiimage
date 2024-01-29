// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.0-or-later

import QtQuick
import QtQuick.Controls as QQC2
import org.kde.kirigami as Kirigami
import QtQuick.Layouts
import org.kde.kirigamiaddons.formcard as FormCard
import org.kde.optiimage.private
import org.kde.coreaddons

FormCard.FormCardPage {
    id: root

    title: i18nc("@title:window", "Settings")

    FormCard.FormHeader {
        title: i18n("General")
    }

    FormCard.FormCard {
        FormCard.FormSwitchDelegate {
            id: keepMetadataCheck
            text: i18n("Keep Metadata")
            checked: Config.keepMetadata
            onCheckedChanged: {
                Config.keepMetadata = checked;
                Config.save();
            }
        }

        FormCard.FormDelegateSeparator {
            above: safeModeCheck
            below: keepMetadataCheck
        }

        FormCard.FormSwitchDelegate {
            id: safeModeCheck
            text: i18n("Safe mode")
            description: i18n("Save the compressed image in a new file")
            checked: Config.safeMode
            onCheckedChanged: {
                Config.safeMode = checked;
                Config.save();
            }
        }

        FormCard.FormDelegateSeparator {
            above: safeModeCheck
        }

        FormCard.FormTextFieldDelegate {
            enabled: safeModeCheck.checked
            label: i18n("Suffix to append at the end of new file")
            text: Config.suffix
            onTextChanged: {
                Config.suffix = text;
                Config.save();
            }
        }
    }

    FormCard.FormHeader {
        title: i18n("JPEG")
    }

    FormCard.FormCard {
        FormCard.FormSwitchDelegate {
            id: jpgProgressiveCheck
            text: i18n("Progressive encode JPEG image")
            description: i18n("Progressive encoded image can improve your website perceived performance.")
            checked: Config.jpgProgressive
            onCheckedChanged: {
                Config.jpgProgressive = checked;
                Config.save();
            }
        }

        FormCard.FormDelegateSeparator {
            above: jpgLosslessCheck
            below: jpgProgressiveCheck
        }

        FormCard.FormSwitchDelegate {
            id: jpgLosslessCheck
            text: i18n("Use lossless compression")
            description: i18n("Only use compression algorithms which do not reduce image quality.")
            checked: Config.jpgLossless
            onCheckedChanged: {
                Config.jpgLossless = checked;
                Config.save();
            }
        }

        FormCard.FormDelegateSeparator {
            above: jpgLosslessCheck
        }

        FormCard.FormSpinBoxDelegate {
            enabled: jpgLosslessCheck.checked
            label: i18n("Default optimization level of lossy optimization:")
            value: Config.jpgLossyLevel
            from: 10
            to: 100
            onTextChanged: {
                Config.jpgLossyLevel = value;
                Config.save();
            }
        }
    }

    FormCard.FormHeader {
        title: i18n("PNG")
    }

    FormCard.FormCard {
        FormCard.FormSpinBoxDelegate {
            label: i18n("Default optimization level of lossless optimization:")
            value: Config.pngLossyLevel
            from: 1
            to: 6
            onTextChanged: {
                Config.pngLossyLevel = value;
                Config.save();
            }
        }
    }

    FormCard.FormCard {
        Layout.topMargin: Kirigami.Units.gridUnit

        FormCard.FormButtonDelegate {
            text: i18n("About OptiImage")
            icon.name: "org.kde.optiimage"
            onClicked: {
                QQC2.ApplicationWindow.window.pageStack.layers.push(aboutPage);
            }

            Component {
                id: aboutPage
                FormCard.AboutPage {
                    aboutData: AboutData
                }
            }
        }

        FormCard.FormDelegateSeparator {}

        FormCard.FormButtonDelegate {
            text: i18n("About KDE")
            icon.name: "kde"
            onClicked: {
                QQC2.ApplicationWindow.window.pageStack.layers.push(aboutKDE);
            }

            Component {
                id: aboutKDE
                FormCard.AboutKDE {}
            }
        }
    }
}