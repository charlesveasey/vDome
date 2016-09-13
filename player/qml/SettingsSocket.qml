import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.1

/**************************************************************
 SOCKET SETTINGS
 **************************************************************/
Item{
    anchors.fill: parent.fill;
    property int columnOffset: 10;
    property int column2x: 250;
    property var hText: hostText;
    property var pText: portText;

    Column{
        y: 0; width: 400; height: 100;
        spacing: columnSpacing;


        Text {
            width: 225;
            color: '#fff'; font.pixelSize: fontSizeHeader; font.family: fontBold; font.bold: true;
            text: qsTr("Socket")
        }

        Text {
            width: 225;
            color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular;
            text: qsTr("Host IP")

            TextInput{
                id:hostText
                x: column2x; width:225;
                color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular
                clip: true; activeFocusOnPress: true; readOnly: false; selectByMouse: true;
                onTextChanged: {
                    socket.host = text;
                }
            }
        }



        Text {
            width: 225;
            color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular;
            text: qsTr("Port")


            TextInput{
                id:portText
                x: column2x; width:225;
                color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular
                clip: true; activeFocusOnPress: true; readOnly: false; selectByMouse: true;
                onTextChanged: {
                    socket.port = text;
                }
            }

        }



    }
}