// SPDX-FileCopyrightText: 2023 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "optimizer.h"
#include "config.h"
#include <QProcess>
#include <QFile>
#include <QCoroProcess>

using namespace Qt::Literals::StringLiterals;

QCoro::Task<void> optimizeJpeg(const Config *config, const ImageInfo &image)
{
    QStringList params1;
    QStringList params2;

    if (config->safeMode()) {
        params1 = {
            u"--max="_s + QString::number(config->jpgLossyLevel()),
            u"-o"_s,
            u"-f"_s,
            u"--stdout"_s,
            image.path.toLocalFile(),
        };
        params2 = {
            u"-o"_s,
            u"-f"_s,
            u"--stdout"_s,
            image.path.toLocalFile(),
        };
    } else {
        params1 = {
            u"--max="_s + QString::number(config->jpgLossyLevel()),
            u"-o"_s,
            u"-f"_s,
            image.path.toLocalFile(),
        };
        params2 = {
            QStringLiteral("-o"),
            QStringLiteral("-f"),
            image.path.toLocalFile(),
        };
    }

    if (config->jpgProgressive()) {
        params1.prepend(QStringLiteral("--all-progressive"));
        params2.prepend(QStringLiteral("--all-progressive"));
    }

    if (!config->keepMetadata()) {
        params1.prepend(QStringLiteral("--strip-all"));
        params2.prepend(QStringLiteral("--strip-all"));
    }

    QProcess proc;
    auto process = qCoro(proc);

    if (config->safeMode()) {
        co_await process.start(u"jpegoptim"_s, config->jpgLossless() ? params1 : params2);
        co_await process.waitForFinished();
        QFile newFile(image.newPath.toLocalFile());
        if (newFile.open(QIODeviceBase::WriteOnly)) {
            newFile.write(proc.readAllStandardOutput());
            newFile.close();
        }
    } else {
        co_await process.start(u"jpegoptim"_s, config->jpgLossless() ? params1 : params2);
        co_await process.waitForFinished();
    }
}

QCoro::Task<void> optimizePng(const Config *config, const ImageInfo &image) {
    QProcess proc;
    auto process = qCoro(proc);
    QStringList oxipngArguments = {
        u"-o"_s,
        QString::number(config->pngLosslessLevel()),
    };

    if (!config->keepMetadata()) {
        oxipngArguments.append(u"--strip"_s);
        oxipngArguments.append(u"safe"_s);
    }

    oxipngArguments.append(image.path.toLocalFile());
    co_await process.start(QStringLiteral("oxipng"), oxipngArguments);

    co_await process.waitForFinished();
    const QByteArray outputUtf8 = proc.readAllStandardError();
    const auto output = QString::fromUtf8(outputUtf8);
}
