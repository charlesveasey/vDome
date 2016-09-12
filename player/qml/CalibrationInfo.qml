import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.1
import QtQuick.Controls.Styles 1.2

/**************************************************************
 CALIBRATION INFO WINDOW
 **************************************************************/
Window {
    id: root
    title: "Calibration Info"
    width: window.width/2; height: window.height+90
    minimumWidth: 400; minimumHeight: 265;
    color: '#111'; opacity: 1
    flags: Qt.Window | Qt.WindowTitleHint | Qt.WindowCloseButtonHint | Qt.WindowSystemMenuHint


    property int fontSizeHeader: 18;
    property int fontSizeNormal: 14;
    property int columnSpacing: 16;

    property string fontRegular: openSansSemiBold.name;
    property string fontBold: openSansExtraBold.name;

    property int column2x: 250;


    Item {
        x: 30; y:30;

    Column{
        spacing: 15;

        Text {
            x: 0; y: 0;
            color: '#fff'; font.pixelSize: fontSizeHeader; font.family: fontBold; font.bold: true;
            text: qsTr("Calibration")
        }


        Text {
            color: '#fff'; font.pixelSize: 12;
            text: qsTr("use the menu on the dome to calibrate the projectors")
        }





        Item { width: 400; height: 1 }




        Text {
            color: '#fff'; font.pixelSize: fontSizeHeader; font.family: fontBold; font.bold: true;
            text: qsTr("Menu")
        }

        Text {
            color: '#fff'; font.pixelSize: 12;
            text: qsTr("use the keyboard to navigate the projector menus")
        }

        CalibrationKeys {
            height: 135;
        }






        Item { width: 400; height: 2 }

        Text {
            color: '#fff'; font.pixelSize: fontSizeHeader; font.family: fontBold; font.bold: true;
            text: qsTr("View")
        }

        Text {
            color: '#fff'; font.pixelSize: 12;
            text: qsTr("use the mouse to change the view")
        }


        Item{
            width: 100; height:80;
            Grid {
                columns:2;
                rows: 6;
                columnSpacing: 25;
                rowSpacing: 8;

                Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true; text: qsTr("left drag") }
                Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular; text: qsTr("Rotate") }
                Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true; text: qsTr("right drag") }
                Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular; text: qsTr("Zoom") }
                Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true; text: qsTr("middle drag") }
                Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular; text: qsTr("Translate") }
            }

            Grid {
                x: 275;
                columns:2;
                rows: 4;
                columnSpacing: 25;
                rowSpacing: 8;

                Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true; text: qsTr("left drag +'t'") }
                Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular; text: qsTr("Translate") }
                Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true; text: qsTr("scroll wheel") }
                Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular; text: qsTr("Zoom") }
            }
        }

        Item { width: 400; height: 1 }


        Text {
            color: '#fff'; font.pixelSize: fontSizeHeader; font.family: fontBold; font.bold: true;
            text: qsTr("Warp Grid")
        }
        Text {
            color: '#fff'; font.pixelSize: 12;
            text: qsTr("use the mouse to warp the grid")
        }
        Grid {
            columns:2;
            rows: 2;
            columnSpacing: 25;
            rowSpacing: 8;

            Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true; text: qsTr("left drag") }
            Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular; text: qsTr("Set Control Points") }
        }



        Item { width: 400; height: 1 }

        Text {
            color: '#fff'; font.pixelSize: fontSizeHeader; font.family: fontBold; font.bold: true;
            text: qsTr("Color Curves")
        }
        Text {
            color: '#fff'; font.pixelSize: 12;
            text: qsTr("use the keyboard to change the color curves")
        }

        Item{
            width: 100; height:80;
            Grid {
                columns:2;
                rows: 2;
                columnSpacing: 25;
                rowSpacing: 8;

                Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true; text: qsTr("left/right") }
                Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular; text: qsTr("Select Control Point") }
            }
            Grid {
                x: 275;
                columns:2;
                rows: 2;
                columnSpacing: 25;
                rowSpacing: 8;

                Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true; text: qsTr("down/up") }
                Text { color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular; text: qsTr("Set Control Point") }
            }
        }

    } // column
    } // item

}////////////////////////////////////////////
