#include "launcher.h"

#include "launcher.h"
#include <QCoreApplication>
#include <QtDebug>

Launcher::Launcher(QObject *parent) : QObject(parent), started_(false)
{
    process_ = new QProcess(this);
    connect(process_,
            SIGNAL(error(QProcess::ProcessError)),
            SLOT(error(QProcess::ProcessError)));
    connect(process_,
            SIGNAL(finished(int, QProcess::ExitStatus)),
            SLOT(finished(int, QProcess::ExitStatus)));
    connect(process_,
            SIGNAL(stateChanged(QProcess::ProcessState)),
            SLOT(stateChanged(QProcess::ProcessState)));
} // end_ctor(ExternalLauncher)

void Launcher::launch(QString program, QStringList argList)
{
    started_ = true;
    process_->start(program, argList);
    process_->waitForFinished(-1); // (c)
    Q_EMIT feedbackLog(process_->readAllStandardOutput());
    process_->close();
} // end Launcher::launch()

void Launcher::error(QProcess::ProcessError error)
{
    /* just feedback some text about the error */
} // end_slot(Launcher::error)

void Launcher::finished(int exitCode, QProcess::ExitStatus status)
{
    started_ = false;
    /* feedback some text about finished */
} // end_slot (Launcher::finished)

void Launcher::stateChanged(QProcess::ProcessState state)
{
    qDebug() << "Luancher::stateChanged" << state;
}
