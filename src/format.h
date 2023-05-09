// SPDX-FileCopyrightText: 2023 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#pragma once

#include <QObject>

class Format : public QObject
{
    Q_OBJECT

public:
    explicit Format(QObject *parent = nullptr);

    Q_INVOKABLE QString formatByteSize(const uint size);
};