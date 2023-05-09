// SPDX-FileCopyrightText: 2023 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "format.h"
#include <KFormat>

Format::Format(QObject *parent)
    : QObject(parent)
{}


QString Format::formatByteSize(const uint size)
{
    KFormat format;
    return format.formatByteSize(size);
}
