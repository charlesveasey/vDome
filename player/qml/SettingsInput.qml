import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.1

/**************************************************************
 INPUT SETTINGS TAB
 **************************************************************/
Item{
    anchors.fill: parent.fill;



    Column{
        x:0;  y:0; width: parent.width
        spacing: columnSpacing;




        Text {
            width: 225;
            color: '#fff'; font.pixelSize: fontSizeHeader; font.family: fontBold; font.bold: true;
            text: qsTr("Input")
        }





        Text {
            width: 225;
            color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular;
            text: qsTr("Flip")

            Switch {
                x: 75; y: 1;
                checked: false;
                onCheckedChanged: if (socket) socket.sendFlip((checked ? "on" : "off"));
            }
        }





        Text {
            width: 225;
            color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular;
            text: qsTr("Scale")

            Slider {
                id: scaleSlider;
                x: 75;  y: 2; width: settings.width-x-settings.sliderInputTextPad;
                value: .5;
                onValueChanged: if (socket) socket.sendScale(value*2.0);
            }

            TextInput {
                x: scaleSlider.width + settings.sliderInputTextPad;  y: 2; width: 25;
                color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular;
                text: (Math.round(scaleSlider.value * 2.0 * 100.0) / 100.0).toFixed(2);
                onAccepted: scaleSlider.value = Number(displayText) / 2.0;
            }
        }






        Text {
            width: 225;
            color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular;
            text: qsTr("Rotate")

            Slider {
                id: rotateSlider;
                x: 75; y: 2; width: settings.width-x-settings.sliderInputTextPad;
                value: .5;
                onValueChanged: if (socket) socket.sendRotate(value * 360 - 180);
            }

            TextInput {
                id: rotateText;
                x: rotateSlider.width + settings.sliderInputTextPad; y: 2; width: 25;
                color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular;
                text: Math.round(rotateSlider.value * 360 - 180);
                onAccepted: rotateSlider.value = (Number(displayText) + 180) / 360;
            }
        }






        Text {
            width: 225;
            color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular;
            text: qsTr("Tilt")

            Slider {
                id: tiltSlider;
                x: 75; y: 2; width: settings.width-x-settings.sliderInputTextPad;
                value: .5;
                onValueChanged: if (socket) socket.sendTilt(value * 360 - 180) ;
            }

            TextInput {
                x: tiltSlider.width + settings.sliderInputTextPad; y: 2; width: 25;
                color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular;
                text: Math.round(tiltSlider.value * 360 - 180);
                onAccepted: tiltSlider.value = (Number(displayText) + 180) / 360;
            }

        }




    } // column

} ////////////////////////////////////////


