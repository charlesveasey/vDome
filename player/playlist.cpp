#include "playlist.h"
#include <QDir>

Playlist::Playlist(QQuickItem *parent) : QQuickItem(parent)
{
}

QString Playlist::getMimeType(QString filename)
{
    mime = data.mimeTypeForFile(filename, QMimeDatabase::MatchExtension);
    return mime.name();
}

QVariant Playlist::parse(QString filename)
{
    QStringList myList;
    QString filepath;
    QUrl url(filename);
    filepath = url.toLocalFile();

    QFile file(filepath);

    if (!file.exists()) {
        qDebug() << "File: " << file.fileName() << "does not exist";
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Couldn't open file: " << file.fileName();
    }

    // check file type
    bool m3uType = false;
    QTextStream stream(&file);
    QString line = stream.readLine();
    if (line.midRef(0,7) == "#EXTM3U" ) {
         //qDebug() << "valid M3U";
         m3uType = true;
    }
    else {
        qDebug() << "invalid file type";
        m3uType = false;
    }

    if (m3uType) {
        while(!stream.atEnd()) {
            QString line = stream.readLine();
            if (line.midRef(0,8) == "#EXTINF:" ) {
                int d1 = line.indexOf(",");
                myList << line.mid(8,d1-8).trimmed();                   // duration
                int d2 = line.indexOf(" - ");
                myList << line.mid(d1+1,d2-d1-1).trimmed();             // author
                myList << line.mid(d2+2,line.count()-d2+2).trimmed();   // title                
                line = stream.readLine();
                myList << line.trimmed();                                // file
                myList << getMimeType(line);                             // mime type
            }
            else {
            }
        }
    }


    file.close();

    return QVariant::fromValue(myList);
}

void Playlist::save(QString filename, QVariant data) {

    QString filepath;
    QUrl url(filename);
    filepath = url.toLocalFile();

    if(filepath.indexOf(".m3u") < 0)
        filepath.append(".m3u");

    qDebug() << "save file: " <<  filepath ;

    QStringList myList = data.toStringList();
    QFile file(filepath);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);

    out << "#EXTM3U \n";
    QString str = "";

    for (int i=0; i<myList.count(); i+=4) {
        str = " \n#EXTINF:";
        str += myList[i] + ",";             // duration
        str += myList[i+1] + " - ";         // author
        str += myList[i+2];                 // title
        str += "\n" + myList[i+3] + "\n";   // file
        out << str;
    }

    file.close();
}


QString Playlist::convertPath(QString filename){
    QString filepath;
    QUrl url(filename);
    filepath = url.toLocalFile();

    QDir file;
    filepath = file.toNativeSeparators(filepath);

    return filepath;
}

QString Playlist::convertToNativePath(QString filename){
    QString filepath;
    QDir file;
    filepath = file.toNativeSeparators(filename);
    return filepath;
}

