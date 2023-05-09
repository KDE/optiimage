// SPDX-FileCopyrightText: 2023 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "optimizer.h"
#include "config.h"
#include <QProcess>
#include <QCoroProcess>

QCoro::Task<qint64> optimizeJpeg(const QUrl &path)
{
    QProcess proc;

    proc.start(QStringLiteral("jpegoptim"), {
        QStringLiteral("--strip-all"),
        QStringLiteral("--totals"),
        path.toLocalFile()
    });
    proc.waitForStarted();

    auto process = qCoro(proc);
    co_await process.waitForFinished();
    const QByteArray outputUtf8 =  proc.readAll();
    const auto output = QString::fromUtf8(outputUtf8);
    qDebug() << output;

    co_return output.split(QStringLiteral(" --> "))[1].split(QChar(' '))[0].toInt();
}

QCoro::Task<qint64> optimizePng(const QUrl &path) {
    auto config = Config::self();
    QProcess proc;
    QStringList oxipng = { QStringLiteral("-o") };
    oxipng.append(QString::number(config->pngLosslessLevel()));
    proc.start(QStringLiteral("oxipng"), {
        QStringLiteral("-o"),

        path.toLocalFile()
    });

    co_await qCoro(proc).waitForFinished();
    const QByteArray outputUtf8 = proc.readAll();
    const auto output = QString::fromUtf8(outputUtf8);

    if (!output.contains("is already optimized")) {
        const auto size = output.split("Output file size = ")[1].split(' ')[0].toInt();
        co_return size;
    }

    co_return -1;
}
