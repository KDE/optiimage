// SPDX-FileCopyrightText: 2023 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#pragma once

#include <QCoroTask>
#include <QUrl>
#include "imagemodel.h"

class ConsoleLog;
class Config;

QCoro::Task<void> optimizePng(const Config *config, const ImageInfo &path, ConsoleLog *log);
QCoro::Task<void> optimizeJpeg(const Config *config, const ImageInfo &path, ConsoleLog *log);
QCoro::Task<void> optimizeSvg(const Config *config, const ImageInfo &path, ConsoleLog *log);
QCoro::Task<void> optimizeWebp(const Config *config, const ImageInfo &path, ConsoleLog *log);
