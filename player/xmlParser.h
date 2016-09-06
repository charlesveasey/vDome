#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QQuickItem>
#include <QDomDocument>

class XMLDataObject {
public:
    QString windowBorder;
    QString windowPosition;
    QString projectorCount;
    QString projectorResolution;
};

class XMLParser : public QQuickItem
{

    Q_OBJECT
public:
    XMLParser(QQuickItem *parent = 0);
    Q_INVOKABLE QVariant load();
    Q_INVOKABLE void save(QVariant data);
    QList<XMLDataObject> list;

private:
    QString readNextText();
    QString errorString();

    QDomDocument xml;
    QString filename;
    QString defaultFilename;
    QString warpFolder;
    QString colorFolder;
    bool fullReset;

};

#endif // XMLPARSER_H
