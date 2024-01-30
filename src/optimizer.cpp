// SPDX-FileCopyrightText: 2023 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "optimizer.h"
#include "config.h"
#include <QProcess>
#include <QFile>
#include <QFileInfo>
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


QCoro::Task<void> optimizeSvg(const Config *config, const ImageInfo &image) {
    QProcess proc;
    auto process = qCoro(proc);

    QString newPath = image.newPath.toLocalFile();
    if (!config->safeMode()) {
        newPath += u".temp"_s;
    }

    QStringList arguments;
    if (config->svgMaximumLevel()) {
        arguments = {
            u"-i"_s,
            image.path.toLocalFile(),
            u"-o"_s,
            newPath,
            u"--enable-viewboxing"_s,
            u"--enable-id-stripping"_s,
            u"--enable-comment-stripping"_s,
            u"--shorten-ids"_s,
            u"--indent=none"_s
        };
    } else {
        arguments = {
            u"-i"_s,
            image.path.toLocalFile(),
            u"-o"_s,
            newPath,
        };
    }

    co_await process.start(u"scour"_s, arguments);
    co_await process.waitForFinished();

    if (!config->safeMode()) {
        QFile newPathFile(newPath);
        newPathFile.rename(image.path.toLocalFile());
    }
}

QCoro::Task<void> optimizeWebp(const Config *config, const ImageInfo &image) {
    QProcess proc;
    auto process = qCoro(proc);

    QStringList arguments = {image.path.toLocalFile()};

    QString newPath = image.newPath.toLocalFile();
    if (!config->safeMode()) {
        newPath += u".temp"_s;
    }

    if (!config->keepMetadata()) {
        arguments.append(u"-metadata"_s);
        arguments.append(u"all"_s);
    }

    int quality = config->webpLossyLevel();
    if (config->webpLossless()) {
        arguments.append(u"-lossless"_s);
        quality = 100;
    }

    // multithreaded
    arguments.append(u"-mt"_s);

    // (lossless) compression mode
    arguments.append(u"-m"_s);
    arguments.append(QString::number(config->webpLosslessLevel()));

    // quality
    arguments.append(u"-q"_s);
    arguments.append(QString::number(quality));

    // output
    arguments.append(u"-o"_s);
    arguments.append(newPath);

    co_await process.start(u"cwebp"_s, arguments);
    co_await process.waitForFinished();

    if (!config->safeMode()) {
        QFileInfo fileInfo(image.path.toLocalFile());
        QFileInfo fileInfoNew(newPath);

        if (fileInfo.size() < fileInfoNew.size()) {
            co_return;
        }

        QFile newPathFile(newPath);
        newPathFile.rename(image.path.toLocalFile());
    }

}
