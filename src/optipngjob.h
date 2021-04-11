// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#pragma once

#include <KJob>
#include <QUrl>

class OptipngJob : public KJob
{
    Q_OBJECT

public:
    OptipngJob(QUrl path, QObject *parent = nullptr);
    ~OptipngJob();
    
    void start() override;
    void optimize();
    
    qint64 newSize();
    
private:
    QString m_output;
    QUrl m_path;
    qint64 m_newSize;
};
