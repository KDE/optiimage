// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include <KAboutData>
#include <KIconTheme>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <QApplication>
#include <QCoroQml>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QUrl>
#include <QtQml>

#include "config.h"

#include "optiimage-version.h"

#if __has_include("KCrash")
#include <KCrash>
#endif

using namespace Qt::Literals::StringLiterals;

int main(int argc, char *argv[])
{
    KIconTheme::initTheme();
    QIcon::setFallbackThemeName(u"breeze"_s);

    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("optiimage");

    // Default to org.kde.desktop style unless the user forces another style
    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }

    KAboutData about(QStringLiteral("optiimage"),
                     i18n("OptiImage"),
                     QStringLiteral(OPTIIMAGE_VERSION_STRING),
                     i18n("Images Optimiser"),
                     KAboutLicense::GPL_V3,
                     i18n("© 2021 - 2023 Carl Schwan"));

    about.addAuthor(i18n("Carl Schwan"), i18n("Maintainer"), QStringLiteral("carl@carlschwan.eu"));
    about.setTranslator(i18nc("NAME OF TRANSLATORS", "Your names"), i18nc("EMAIL OF TRANSLATORS", "Your emails"));
    about.setOrganizationDomain("kde.org");
    about.setBugAddress("https://bugs.kde.org/describecomponents.cgi?product=optiimage");

    KAboutData::setApplicationData(about);
    QGuiApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("org.kde.optiimage")));

#if __has_include("KCrash")
    KCrash::initialize();
#endif

    auto config = Config::self();
    qmlRegisterSingletonInstance("org.kde.optiimage.private", 1, 0, "Config", config);

    QQmlApplicationEngine engine;
    QCoro::Qml::registerTypes();

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.loadFromModule(u"org.kde.optiimage"_s, u"Main"_s);

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
