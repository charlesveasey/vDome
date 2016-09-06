#include "xmlParser.h"
#include <QDir>
#include "syscmds.h"

XMLParser::XMLParser(QQuickItem *parent) : QQuickItem(parent){
    QString path = QCoreApplication::applicationDirPath();
    filename = QDir::toNativeSeparators(path + "/renderer/data/settings.xml");
    defaultFilename = QDir::toNativeSeparators(path + "/renderer/data/default.xml");
    warpFolder = QDir::toNativeSeparators(path + "/renderer/data/settings/warp/");
    colorFolder = QDir::toNativeSeparators(path + "/renderer/data/settings/color/");
    fullReset = false;
}

QVariant XMLParser::load(){

    // load file
    QStringList list;
    QDomDocument doc;
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return QVariant::fromValue(list);


    // parse xml
     QDomElement element;
     QString attr;
     int pCount = 0;


     // window xml
     QDomNodeList window = doc.elementsByTagName("window");

     for (int i = 0; i < window.size(); i++) {

         // reset projector count
         pCount = 0;

         // window index
         list << "window" << QString::number(i);

         // border
         element = window.at(i).toElement();
         attr = element.attribute("border");
         list << "border" << attr;

         // position
         element = window.at(i).toElement();
         attr = element.attribute("position");
         list << "position" << attr;

         // projector
         QDomElement projectors = element;
         for (QDomElement p = projectors.firstChildElement("projector"); !p.isNull(); p = p.nextSiblingElement("projector")){
             pCount++;

             attr = p.attribute("resolution");
         }

         // projector count
         list << "pCount" << QString::number(pCount);

         // resolution
         list << "resolution" << attr;
    }

    return QVariant::fromValue(list);
}

void XMLParser::save(QVariant data){
    fullReset = false;
    // load default file
    QStringList list;
    QDomDocument doc;
    QFile file(defaultFilename);

    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return;

    QDomNode element;
    QString attr;
    QDomNode pClone;
    QDomNode wClone;
    QDomNodeList p = doc.elementsByTagName("projector");
    pClone = p.at(0).cloneNode();
    QDomNodeList w = doc.elementsByTagName("window");
    wClone = w.at(0).cloneNode(false);
    QDomNode v = doc.firstChild();

    v.removeChild(w.at(0));

    QStringList dataList = data.toStringList();

    for (int i=0; i<dataList.count(); i++) {
        if (dataList[i] == "window"){
            QDomElement clone = wClone.cloneNode(false).toElement();

            QString b = (dataList[i+3] == "true") ? "on" : "off";
            clone.setAttribute("border", b);
            clone.setAttribute("position", dataList[i+5]);

            int pCount = dataList[i+7].toInt();

            int resx = dataList[i+9].toInt();
            int resy = dataList[i+11].toInt();
            clone.setAttribute("resolution", ( QString::number(resx*pCount) + "," + QString::number(resy) ) );

            for (int j=0; j<pCount; j++){
                QDomElement clone2 = pClone.cloneNode().toElement();
                QString res = dataList[i+9] + "," + dataList[i+11];
                clone2.setAttribute("resolution", res);
                clone.appendChild(clone2);
            }

            fullReset = (dataList[i+13] == "true") ? true : false;
            v.appendChild(clone);
        }
    }

    QFile fileOut( filename );
    if ( !fileOut.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
       qDebug( "Failed to open file for writing." );
    }

    QTextStream stream( &fileOut );
    stream << doc.toString();
    fileOut.close();

    if (fullReset){
        // remove settings directories for full reset
        Syscmds::removeDir(warpFolder);
        Syscmds::removeDir(colorFolder);
    }
}



