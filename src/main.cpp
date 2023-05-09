// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
#include <QQuickStyle>
#include <QCoroQml>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <KAboutData>

#include "imagemodel.h"
#include "format.h"

#include "optiimage-version.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("optiimage");

    // Default to org.kde.desktop style unless the user forces another style
    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }

    KAboutData about(QStringLiteral("optiimgage"),
                    i18n("OptiImage"),
                    QStringLiteral(OPTIIMAGE_VERSION_STRING),
                    i18n("Images Optimiser"),
                    KAboutLicense::GPL_V3,
                    i18n("© 2021 Carl Schwan"));

    about.addAuthor(i18n("Carl Schwan"), QString(), QStringLiteral("carl@carlschwan.eu"));
    about.setOrganizationDomain("kde.org");

    QQmlApplicationEngine engine;
    qmlRegisterType<ImageModel>("org.kde.optiimage", 1, 0, "ImageModel");
    qmlRegisterSingletonType("org.kde.optiimage", 1, 0, "About", [](QQmlEngine *engine, QJSEngine *) -> QJSValue {
        return engine->toScriptValue(KAboutData::applicationData());
    });
    qmlRegisterSingletonType<Format>("org.kde.optiimage", 1, 0, "Format", [](QQmlEngine *, QJSEngine *) -> QObject * {
        return new Format();
    });
    QCoro::Qml::registerTypes();

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
