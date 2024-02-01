// SPDX-FileCopyrightText: 2024 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#pragma once

#include <QAbstractListModel>
#include <QQmlEngine>

class ConsoleLog : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    enum ExtraRoles {
        OutputRole = Qt::UserRole + 1,
        ImagePathRole,
        ImageNameRole,
    };

    explicit ConsoleLog(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addConsoleEntry(const QString &output, const QString &imagePath);

private:
    struct LogEntry
    {
        QString output;
        QString imagePath;
    };

    std::vector<LogEntry> m_logEntries;
};
