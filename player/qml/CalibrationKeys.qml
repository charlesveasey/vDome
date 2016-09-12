import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.1
import QtQuick.Controls.Styles 1.2

/**************************************************************
 CALIBRATION KEYS
 **************************************************************/

Item{
    width: 300; height: 300
    property int fontSizeNormal: 14;


    Grid {
        x: 0;
        columns:2;
        rows: 10;
        columnSpacing: 25;
        rowSpacing: 8;

        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true; text: qsTr("m") }
        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular; text: qsTr("Menu") }
        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true; text: qsTr("down/up") }
        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular; text: qsTr("Navigate") }
        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true; text: qsTr("back/del") }
        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular; text: qsTr("Back") }
        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true; text: qsTr("left/right") }
        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular; text: qsTr("Value") }
        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true; text: qsTr("alt") }
        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular; text: qsTr("Scale Value") }
    }

    Grid {
        x: 275;
        columns:2;
        rows: 10;
        columnSpacing: 25;
        rowSpacing: 8;

        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true; text: qsTr("1 - 9") }
        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular; text: qsTr("Projector") }
        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true; text: qsTr("`") }
        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular; text: qsTr("All") }
        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true; text: qsTr("cmd + s") }
        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular; text: qsTr("Save") }
        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true; text: qsTr("r") }
        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular; text: qsTr("Reset") }
        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true; text: qsTr("z/y") }
        Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular; text: qsTr("Undo/Redo") }

    }


}////////////////////////////////////////////
