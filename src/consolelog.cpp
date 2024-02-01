// SPDX-FileCopyrightText: 2024 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "consolelog.h"

#include <QFileInfo>

ConsoleLog::ConsoleLog(QObject *parent)
    : QAbstractListModel(parent)
{
}

QVariant ConsoleLog::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(checkIndex(index, QAbstractItemModel::CheckIndexOption::IndexIsValid));

    const auto &logEntry = m_logEntries[index.row()];
    switch (role) {
    case Qt::DisplayRole:
    case OutputRole:
        return logEntry.output;
    case ImagePathRole:
        return logEntry.imagePath;
    case ImageNameRole:
        return QFileInfo(logEntry.imagePath).fileName();
    default:
        return {};
    }
}

int ConsoleLog::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_logEntries.size();
}

QHash<int, QByteArray> ConsoleLog::roleNames() const
{
    return {
        {OutputRole, "output"},
        {ImagePathRole, "imagePath"},
        {ImageNameRole, "imageName"},
    };
}

void ConsoleLog::addConsoleEntry(const QString &output, const QString &imagePath)
{
    m_logEntries.emplace_back(output, imagePath);
}
