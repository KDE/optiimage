// SPDX-FileCopyrightText: 2021 Carl Schwan <carlschwan@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "optipngjob.h"
#include <QProcess>
#include <QTimer>

OptipngJob::OptipngJob(QUrl path, QObject *parent)
    : AbstractOptiJob(parent)
    , m_path(path)
{
}

OptipngJob::~OptipngJob()
{
}

void OptipngJob::start()
{
     QTimer::singleShot(0, this, &OptipngJob::optimize);
}

void OptipngJob::optimize()
{
    QProcess *proc = new QProcess;
    proc->start(QStringLiteral("optipng"), {QStringLiteral("-o3 "), m_path.toLocalFile()});
    connect(proc, &QProcess::readyReadStandardError, this, [this, proc]() {
        m_output += proc->readAllStandardError();
    });
        
    connect(proc,  QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this, proc](int exitCode, QProcess::ExitStatus exitStatus) {
        if (!m_output.contains("is already optimized")) {
            m_newSize = m_output.split("Output file size = ")[1].split(QChar(' '))[0].toInt();
        }
        Q_EMIT emitResult();
        proc->deleteLater();
    });
}

qint64 OptipngJob::newSize()
{
    return m_newSize;
}



