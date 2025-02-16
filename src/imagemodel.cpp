// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "imagemodel.h"

#include "config.h"
#include "consolelog.h"
#include "optimizer.h"
#include <KIO/JobUiDelegateFactory>
#include <KIO/OpenFileManagerWindowJob>
#include <KIO/OpenUrlJob>
#include <KPropertiesDialog>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMimeDatabase>

using namespace Qt::Literals::StringLiterals;

ImageModel::ImageModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QVariant ImageModel::data(const QModelIndex &index, int role) const
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

int ImageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_images.count();
}

void ImageModel::addImages(const QList<QUrl> &paths)
{
    const auto config = Config::self();
    for (const auto &path : paths) {
        bool imageExists = false;
        if (!m_images.isEmpty()) {
            for (const auto &image : m_images) {
                if (image.path == path) {
                    imageExists = true;
                    break;
                }
            }
        }
        if (imageExists) {
            continue;
        }
        beginInsertRows({}, m_images.count(), m_images.count());
        QFileInfo fileInfo(path.toLocalFile());
        QMimeDatabase db;
        QMimeType type = db.mimeTypeForFile(path.toLocalFile());

        ImageInfo info;
        info.path = path;
        if (config->safeMode()) {
            const QString extension = fileInfo.suffix();
            if (!extension.isEmpty()) {
                const QString newFileName = fileInfo.completeBaseName() + config->suffix() + u"." + extension;
                info.newPath = QUrl::fromLocalFile(fileInfo.dir().absoluteFilePath(newFileName));
            } else {
                info.newPath = QUrl(info.path.toString() + config->suffix());
            }
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

void ImageModel::removeImage(const int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    m_images.removeAt(index);
    endRemoveRows();
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
        Q_EMIT dataChanged(index(i, 0), index(i, 0), {NewSizeRole, ProcessedRole, AlreadyOptimizedRole});

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
    return {{Qt::DisplayRole, QByteArrayLiteral("displayName")},
            {FileNameRole, QByteArrayLiteral("filename")},
            {SizeRole, QByteArrayLiteral("size")},
            {NewSizeRole, QByteArrayLiteral("newSize")},
            {AlreadyOptimizedRole, QByteArrayLiteral("alreadyOptimized")},
            {ProcessedRole, QByteArrayLiteral("processed")}};
}

bool ImageModel::running() const
{
    return m_running;
}

void ImageModel::highlightInFileManager(const QString &url)
{
    KIO::highlightInFileManager({QUrl(url)});
}

void ImageModel::open(const QString &url)
{
    auto job = new KIO::OpenUrlJob(QUrl(url));
    job->setUiDelegate(KIO::createDefaultJobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, nullptr));
    job->start();
}

void ImageModel::openProperties(const QString &url)
{
    auto dialog = new KPropertiesDialog({QUrl(url)});
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setModal(true);
    dialog->show();
}
