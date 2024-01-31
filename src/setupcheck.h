// SPDX-FileCopyrightText: 2024 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#pragma once

#include <QObject>
#include <QtQml>

class SetupCheck : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool isValidSetup READ isValidSetup NOTIFY isValidSetupChanged)
    Q_PROPERTY(QStringList missingPrograms READ missingPrograms NOTIFY missingProgramsChanged)

public:
    explicit SetupCheck(QObject *parent = nullptr);

    bool isValidSetup() const;
    QStringList missingPrograms() const;

    Q_INVOKABLE void check();

Q_SIGNALS:
    bool isValidSetupChanged();
    bool missingProgramsChanged();

private:
    bool m_isValidSetup = true;
    QStringList m_missingPrograms;
};