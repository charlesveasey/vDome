#ifndef SYSCMDS_H
#define SYSCMDS_H

#include <QQuickItem>
#include <QProcess>

class Syscmds : public QQuickItem
{
    Q_OBJECT
public:
    Syscmds(QQuickItem *parent = 0);
    Q_INVOKABLE void deleteFile(QString filename);
    Q_INVOKABLE void startRenderer();
    Q_INVOKABLE void closeRenderer();
    Q_INVOKABLE void restartRenderer();
    Q_INVOKABLE QString getOSName();
    static bool removeDir(const QString &dirName);

private:
    void processError(QProcess::ProcessError err);

    QProcess Process;


public slots:
    void processExited();
    void onStarted();

};

#endif // SYSCMDS_H
