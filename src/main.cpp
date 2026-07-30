// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include <KAboutData>
#include <KIconTheme>
#include <KLocalizedQmlContext>
#include <KLocalizedString>
#include <KirigamiAddons/App/KirigamiAppDefaults>
#include <QApplication>
#include <QCoroQml>
#include <QIcon>
#include <QQuickStyle>
#include <QtQml>

#include "optiimage-version.h"

using namespace Qt::Literals::StringLiterals;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("optiimage");

    KirigamiAppDefaults::apply(&app);

    KAboutData about(QStringLiteral("optiimage"),
                     i18n("OptiImage"),
                     QStringLiteral(OPTIIMAGE_VERSION_STRING),
                     i18n("Images Optimiser"),
                     KAboutLicense::GPL_V3,
                     i18n("© 2021 - 2023 Carl Schwan"));

    about.addAuthor(i18n("Carl Schwan"), i18n("Maintainer"), QStringLiteral("carl@carlschwan.eu"));
    about.setTranslator(i18nc("NAME OF TRANSLATORS", "Your names"), i18nc("EMAIL OF TRANSLATORS", "Your emails"));
    about.setOrganizationDomain("kde.org");

    KAboutData::setApplicationData(about);
    QGuiApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("org.kde.optiimage")));

    QQmlApplicationEngine engine;
    QCoro::Qml::registerTypes();

    KLocalization::setupLocalizedContext(&engine);
    engine.loadFromModule(u"org.kde.optiimage"_s, u"Main"_s);

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
