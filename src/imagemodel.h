// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#pragma once

#include <QAbstractListModel>
#include <QUrl>
#include <QList>
#include <QCoroQmlTask>
#include <QQmlEngine>

enum ImageType {
    PNG,
    JPEG,
    SVG,
    WEB,
    UNSURPORTED,
};

struct ImageInfo {
    QUrl path;
    QUrl newPath;
    qint64 size = -1;
    qint64 oldSize = -1;
    ImageType imageType;
    bool processed = false;
    bool error = false;
};

class ImageModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)

public:
    enum Roles {
        FileNameRole = Qt::UserRole,
        SizeRole,
        NewSizeRole,
        SavingRole,
        AlreadyOptimizedRole,
        ProcessedRole
    };

public:
    explicit ImageModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE void addImages(const QList<QUrl> &paths);

    bool running() const;

Q_SIGNALS:
    void runningChanged();

private:
    QCoro::Task<> optimize();

    QVector<ImageInfo> m_images;
    bool m_running = false;
};
