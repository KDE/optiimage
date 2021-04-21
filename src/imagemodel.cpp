// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "imagemodel.h"

#include <QMimeDatabase>    
#include <QDebug>
#include <QFileInfo>

#include "optipngjob.h"
#include "jpegoptimjob.h"

ImageModel::ImageModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

ImageModel::~ImageModel()
{
}

QVariant ImageModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return {};
    }
    
    const auto imageInfo = m_images[index.row()];
    switch (role) {
        case Qt::DisplayRole:
            return imageInfo.path.fileName();
        case FileNameRole:
            return imageInfo.path.toDisplayString();
        case NewSizeRole:
            return imageInfo.size != -1 && imageInfo.size != imageInfo.oldSize ? imageInfo.size : QVariant();
        case AlreadyOptimizedRole:
            return imageInfo.size != -1 && imageInfo.size == imageInfo.oldSize;
        case SizeRole:
            return imageInfo.oldSize != -1 ? imageInfo.oldSize : QVariant();
        case ProcessedRole:
            return imageInfo.processed;
        
    }
    return {};
}


int ImageModel::rowCount(const QModelIndex& parent) const
{
    return m_images.count();
}

void ImageModel::addImages(const QList<QUrl> &paths)
{
    for (const auto &path : paths) {
        beginInsertRows(QModelIndex(), m_images.count(), m_images.count());
        QFileInfo fileInfo(path.toLocalFile());
        QMimeDatabase db;
        QMimeType type = db.mimeTypeForFile(path.toLocalFile());       
        
        ImageInfo info;
        info.path = path;
        info.oldSize = fileInfo.size();
        if (!path.isLocalFile() || !fileInfo.isWritable()) {
            info.error = true;
        }
        
        if (type.name() == QStringLiteral("image/png")) {
            info.imageType = ImageType::PNG;
        } else if (type.name() == QStringLiteral("image/jpeg")) {
            info.imageType = ImageType::JPEG;
        } else {
            info.imageType = ImageType::UNSURPORTED;
        }
        m_images << info;
        endInsertRows();
    }
}

void ImageModel::optimize(bool next)
{
    if (m_running && !next) {
        // We are already running
        return;
    }
    if (!m_running && next) {
        // User asked to stop
        return;
    }
    m_running = true;
    Q_EMIT runningChanged();
    int i = 0;
    for (auto &image : m_images) {
        if (image.processed) {
            i++;
            continue;
        }
        
        AbstractOptiJob *job = nullptr;
        if (image.imageType == ImageType::PNG) {
            job = new OptipngJob(image.path, this);
        } else if (image.imageType == ImageType::JPEG) {
            job = new JpegOptimJob(image.path, this);
        }
        if (job) {
            connect(job, &OptipngJob::result, this, [this, i, job, &image](KJob *) {
                if (job->newSize() != -1) {
                    image.size = job->newSize();
                } else {
                    image.size = image.oldSize;
                }
                image.processed = true;
                Q_EMIT dataChanged(index(i, 0), index(i, 0), { NewSizeRole, ProcessedRole, AlreadyOptimizedRole});
                optimize(true);
                job->deleteLater();
            });
            job->start();
            return;
        }
        i++;
    }
    m_running = false;
    Q_EMIT runningChanged();
}

QHash<int, QByteArray> ImageModel::roleNames() const
{
    return {
        {Qt::DisplayRole, QByteArrayLiteral("display")},
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

void ImageModel::stop()
{
    m_running = false;
    Q_EMIT runningChanged();
}


