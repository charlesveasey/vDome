#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QQuickItem>
#include <QMimeDatabase>

class DataObject {
public:
    QString title;
    QString author;
    QString duration;
    QString file;
};

class Playlist : public QQuickItem
{
    Q_OBJECT
public:
    Playlist(QQuickItem *parent = 0);
    QList<DataObject> list;

    Q_INVOKABLE QVariant parse(QString filename);
    Q_INVOKABLE void save(QString filename, QVariant data);
    Q_INVOKABLE QString convertPath(QString filename);
    Q_INVOKABLE QString getMimeType(QString filename);
    Q_INVOKABLE QString convertToNativePath(QString filename);

private:
    QMimeDatabase data;
    QMimeType mime;

};

#endif // PLAYLIST_H
