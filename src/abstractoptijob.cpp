// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "abstractoptijob.h"

AbstractOptiJob::AbstractOptiJob(QObject *parent)
    : KJob(parent)
{
}

AbstractOptiJob::~AbstractOptiJob()
{
}
