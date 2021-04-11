// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "jpegoptimjob.h"
#include <QProcess>
#include <QTimer>

JpegOptimJob::JpegOptimJob(const QUrl &path, QObject *parent)
    : KJob(parent)
    , m_path(path)
{
}

JpegOptimJob::~JpegOptimJob()
{
}

void JpegOptimJob::start()
{
     QTimer::singleShot(0, this, &JpegOptimJob::optimize);
}

void JpegOptimJob::optimize()
{
    QProcess *proc = new QProcess;
    proc->start(QStringLiteral("jpegoptim"), {QStringLiteral("--strip-all"), QStringLiteral("--totals"), m_path.toLocalFile()});
    
    connect(proc, &QProcess::readyReadStandardOutput, this, [this, proc]() {
        m_output += proc->readAllStandardOutput();
    });
        
    connect(proc,  QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this, proc](int exitCode, QProcess::ExitStatus exitStatus) {
        m_newSize = m_output.split(QStringLiteral(" --> "))[1].split(QChar(' '))[0].toInt();
        Q_EMIT emitResult();
        proc->deleteLater();
    });
}

qint64 JpegOptimJob::newSize()
{
    return m_newSize;
}

