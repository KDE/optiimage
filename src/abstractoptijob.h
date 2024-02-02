// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#pragma once

#include <KJob>
#include <QUrl>

class AbstractOptiJob : public KJob
{
    Q_OBJECT

public:
    AbstractOptiJob(QObject *parent = nullptr);
    ~AbstractOptiJob();
    virtual qint64 newSize() = 0;
};
