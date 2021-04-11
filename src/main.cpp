// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <KAboutData>

#include "imagemodel.h"

#include "optiimage-version.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    
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

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
    
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
