// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "imagemodel.h"

#include <QMimeDatabase>
#include <QDebug>
#include <QFileInfo>
#include "optimizer.h"
#include "consolelog.h"
#include "config.h"

using namespace Qt::Literals::StringLiterals;

ImageModel::ImageModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QVariant ImageModel::data(const QModelIndex& index, int role) const
{
    Q_ASSERT(checkIndex(index, QAbstractItemModel::CheckIndexOption::IndexIsValid));

    const auto &imageInfo = m_images[index.row()];
    switch (role) {
    case Qt::DisplayRole:
        return imageInfo.path.fileName();
    case FileNameRole:
        return imageInfo.path.toDisplayString();
    case NewSizeRole:
        return imageInfo.size != -1 && imageInfo.size != imageInfo.oldSize ? imageInfo.size : -1;
    case AlreadyOptimizedRole:
        return imageInfo.size != -1 && imageInfo.size == imageInfo.oldSize;
    case SizeRole:
        return imageInfo.oldSize != -1 ? imageInfo.oldSize : QVariant();
    case ProcessedRole:
        return imageInfo.processed;
    default:
        return {};
    }
}


int ImageModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_images.count();
}

void ImageModel::addImages(const QList<QUrl> &paths)
{
    const auto config = Config::self();
    for (const auto &path : paths) {
        beginInsertRows({}, m_images.count(), m_images.count());
        QFileInfo fileInfo(path.toLocalFile());
        QMimeDatabase db;
        QMimeType type = db.mimeTypeForFile(path.toLocalFile());

        ImageInfo info;
        info.path = path;
        if (config->safeMode()) {
            info.newPath = QUrl(info.path.toString() + config->suffix());
        } else {
            info.newPath = path;
        }
        info.oldSize = fileInfo.size();
        if (!path.isLocalFile() || !fileInfo.isWritable()) {
            info.error = true;
        }

        if (type.name() == u"image/png"_s) {
            info.imageType = ImageType::PNG;
        } else if (type.name() == u"image/jpeg"_s) {
            info.imageType = ImageType::JPEG;
        } else if (type.name() == u"image/svg+xml"_s) {
            info.imageType = ImageType::SVG;
        } else if (type.name() == u"image/webp"_s) {
            info.imageType = ImageType::WEBP;
        } else {
            info.imageType = ImageType::UNSURPORTED;
        }
        m_images << info;
        endInsertRows();
    }

    optimize();
}

QCoro::Task<> ImageModel::optimize()
{
    if (m_running) {
        co_return;
    }
    m_running = true;
    Q_EMIT runningChanged();

    bool processedImages = false;

    const auto &config = Config::self();
    auto engine = qmlEngine(this);
    auto consoleLog = engine->singletonInstance<ConsoleLog *>(u"org.kde.optiimage"_s, u"ConsoleLog"_s);
    Q_ASSERT(consoleLog);

    int i = 0;
    for (auto &image : m_images) {
        if (image.processed) {
            i++;
            continue;
        }
        image.processed = true;
        processedImages = true;

        if (image.imageType == ImageType::UNSURPORTED) {
            i++;
            continue;
        }

        if (image.imageType == ImageType::PNG) {
            co_await optimizePng(config, image, consoleLog);
        } else if (image.imageType == ImageType::JPEG) {
            co_await optimizeJpeg(config, image, consoleLog);
        } else if (image.imageType == ImageType::SVG) {
            co_await optimizeSvg(config, image, consoleLog);
        } else if (image.imageType == ImageType::WEBP) {
            co_await optimizeWebp(config, image, consoleLog);
        }

        QFileInfo fileInfo(image.newPath.toLocalFile());
        image.size = fileInfo.size();
        image.processed = true;
        Q_EMIT dataChanged(index(i, 0), index(i, 0), { NewSizeRole, ProcessedRole, AlreadyOptimizedRole});

        i++;
    }
    m_running = false;
    Q_EMIT runningChanged();

    if (processedImages) {
        // In case new images still need to be looped over
        optimize();
    }
}

QHash<int, QByteArray> ImageModel::roleNames() const
{
    return {
        {Qt::DisplayRole, QByteArrayLiteral("displayName")},
        {FileNameRole, QByteArrayLiteral("filename")},
        {SizeRole, QByteArrayLiteral("size")},
        {NewSizeRole, QByteArrayLiteral("newSize")},
        {AlreadyOptimizedRole, QByteArrayLiteral("alreadyOptimized")},
        {ProcessedRole, QByteArrayLiteral("processed")}
    };
}

bool ImageModel::running() const
{
    return m_running;
}
