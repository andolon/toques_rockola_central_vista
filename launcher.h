#ifndef LAUNCHER_H
#define LAUNCHER_H


//#include <QCoreApplication>
//#include <QtDebug>
#include <QObject>
#include <QProcess>


class Launcher : public QObject
{
    Q_OBJECT
public:
    /** constructor */
    explicit Launcher(QObject *parent = 0);
    /** destructor */
    ~Launcher() {
        if (started_ && process_->state() != QProcess::NotRunning)
            process_->kill();
    } // end_dtor(Launcher)
Q_SIGNALS:
    void feedbackLog(QString log);
public Q_SLOTS:
    void launch(QString program, QStringList argList);
private:
    QProcess * process_;
    bool started_;
private Q_SLOTS:
    void error(QProcess::ProcessError error);
    void finished(int exitCode, QProcess::ExitStatus status);
    void stateChanged(QProcess::ProcessState state);
}; // end_class(Launcher)

#endif // LAUNCHER_H
