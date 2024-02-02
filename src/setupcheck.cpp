// SPDX-FileCopyrightText: 2024 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "setupcheck.h"

#include <QStandardPaths>

using namespace Qt::Literals::StringLiterals;

SetupCheck::SetupCheck(QObject *parent)
    : QObject(parent)
{
    check();
}

void SetupCheck::check()
{
    m_missingPrograms.clear();

    const QStringList programs = {
        u"oxipng"_s,
        u"scour"_s,
        u"jpegoptim"_s,
        u"cwebp"_s,
    };

    for (const auto &program : programs) {
        if (QStandardPaths::findExecutable(program).isEmpty()) {
            m_missingPrograms << program;
        }
    }

    m_isValidSetup = m_missingPrograms.isEmpty();
    Q_EMIT isValidSetupChanged();
    Q_EMIT missingProgramsChanged();
}

bool SetupCheck::isValidSetup() const
{
    return m_isValidSetup;
}

QStringList SetupCheck::missingPrograms() const
{
    return m_missingPrograms;
}
