// SPDX-FileCopyrightText: 2023 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#pragma once

#include <QCoroTask>
#include <QUrl>

QCoro::Task<qint64> optimizePng(const QUrl &path);
QCoro::Task<qint64> optimizeJpeg(const QUrl &path);
