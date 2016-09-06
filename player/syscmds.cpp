#include "syscmds.h"
#include <QDir>

Syscmds::Syscmds(QQuickItem *parent) : QQuickItem(parent){

}

void Syscmds::deleteFile(QString filename) {
    QFile::remove(filename);

}

void Syscmds::startRenderer() {
    //connect(&Process,SIGNAL(stateChanged(QProcess::ProcessState)),this,SLOT(onStarted()));

    Process.close();

#ifdef Q_OS_WIN
     QString path = QCoreApplication::applicationDirPath();
     Process.start( QDir::toNativeSeparators((path + "/renderer/vDome.exe")) );
#endif

#ifdef Q_OS_MAC
     Process.start("open ./renderer/vDome.app");
#endif

}

void Syscmds::closeRenderer() {
    Process.close();

#ifdef Q_OS_MAC
    Process.start("bash", QStringList() << "-c" << "kill $(ps aux | grep /renderer/vDome | awk '{print $2}')");
#endif

}

void Syscmds::restartRenderer() {
    Process.close();

#ifdef Q_OS_WIN
    Process.start();
#endif

#ifdef Q_OS_MAC
    Process.start( "sh ./renderer/restart.sh");
#endif
}

void Syscmds::onStarted(){
    //qDebug () << Process.state();
}


void Syscmds::processExited()
{
     /*if ( Process->exitStatus() == 0)
    {
        qDebug () << "Program ran successfully";
    }
   if ( process->exitStatus() == 2)
    {
        qDebug () << "Customized message";
    }

    if ( process->exitStatus() == 3)
    {
        qDebug () << "Another text warning message";
    }*/
}

/* Function to get the OS detail based on which
  qml components can be shown*/

QString Syscmds::getOSName(){
    QString osName = "unknown";

#ifdef Q_OS_MAC
     osName =  "osx";
#endif

#ifdef Q_OS_WIN
     osName =  "win";
#endif

     return osName;
}


bool Syscmds::removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }

    return result;
}
