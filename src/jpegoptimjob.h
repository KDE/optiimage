// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#pragma once
#include <KJob>
#include <QUrl>
class JpegOptimJob : public KJob
{
    Q_OBJECT

public:
    JpegOptimJob(const QUrl &path, QObject *parent = nullptr);
    ~JpegOptimJob();
    void start() override;
    void optimize();

    qint64 newSize();
private:
    QString m_output;
    qint64 m_newSize;
    QUrl m_path;
    
};
