#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "playlist.h"
#include "syscmds.h"
#include "xmlParser.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Playlist>("Playlist", 1, 0, "Playlist");
    qmlRegisterType<Syscmds>("Syscmds", 1, 0, "Syscmds");
    qmlRegisterType<XMLParser>("XMLParser", 1, 0, "XMLParser");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));

    return app.exec();
}
