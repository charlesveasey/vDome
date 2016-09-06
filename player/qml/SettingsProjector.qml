import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.1

/**************************************************************
 PROJECTOR SETTINGS
 **************************************************************/
Item{
    anchors.fill: parent.fill;
    property int column2x: 90
    property var pSlider: projectorSlider;

    Column{
        x:0; y: 0; width: parent.width;
        spacing: columnSpacing;



        Text {
            width: 225;
            color: '#fff'; font.pixelSize: fontSizeHeader; font.family: fontBold; font.bold: true;
            text: qsTr("Projector")
        }



        Text {
            width: 225;
            color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular;
            text: qsTr("Menu")

            Switch {
                x: column2x;
                checked: false;
                onCheckedChanged: {
                    if (socket) {
                        socket.sendProjectorMenu((checked ? "on" : "off"));
                     }
                }
            }
        }


    } // column 1



    Column{
        x:0; y: 100; width: parent.width;
        spacing: columnSpacing;


        Text {
            width: 225;
            color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true;
            text: qsTr("Projector Index")
        }



        Slider {
            id: projectorSlider;
            x: 0; y: 0; width: settings.width-x-settings.sliderInputTextPad
            value: 0; maximumValue: 5.0; stepSize: 1;

            TextInput {
                id: projectorInput;
                x: parent.width+25; y: -2; width: 25;
                color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular;
                text: projectorSlider.value + 1;
                onAccepted: projectorSlider.value = Number(displayText) + 1;
            }
        }




        Text {
            width: 225;
            color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular;
            text: qsTr("Enable")

            Switch {
                x: column2x;
                checked: true;
                onCheckedChanged: {
                    if (socket)
                        socket.sendProjectorEnable(projectorSlider.value +","+
                                                   (checked ? "on" : "off"))
                }
            }
        }






        Text {
            width: 225;
            color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular;
            text: qsTr("Azimuth")

            Slider {
                id: polarAzimuthSlider;
                x: column2x; y: 2; width: settings.width-x-settings.sliderInputTextPad
                value: 0;
                onValueChanged: {
                    if (socket)
                        socket.sendProjectorPolar(projectorSlider.value +","+
                                                  ((Number(polarAzimuthSlider.value) * 360)) +","+
                                                  ((Number(polarElevationSlider.value) * 90)) );
                }
            }

            TextInput {
                id: polarAzimuthInput;
                x: polarAzimuthSlider.width + settings.sliderInputTextPad;
                y: 2; width: 25;
                color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular;
                text: Math.round(polarAzimuthSlider.value * 360);
                onAccepted: polarAzimuthSlider.value = (Number(displayText)) / 360;
            }
        }







        Text {
            width: 225;
            color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular;
            text: qsTr("Elevation")

            Slider {
                id: polarElevationSlider;
                x: column2x; y: 2; width: settings.width-x-settings.sliderInputTextPad;
                value: 0;
                onValueChanged: {
                    if (socket)
                        socket.sendProjectorPolar(projectorSlider.value +","+
                                                  ((Number(polarAzimuthSlider.value) * 360)) +","+
                                                  ((Number(polarElevationSlider.value) * 90)) );
                }
            }

            TextInput {
                id: polarElevationInput;
                x: polarElevationSlider.width + settings.sliderInputTextPad; y: 2; width: 25;
                color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular;
                text: Math.round(polarElevationSlider.value * 90);
                onAccepted: polarElevationSlider.value = (Number(displayText)) / 90;
            }
        }


    } // column 2






    Column{
        x:0; y: 310; width: parent.width;
        spacing: columnSpacing;




        Text {
            y: 100; width: 225;
            color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontBold; font.bold: true;
            text: qsTr("Calibration")

            Button {
                 text: qsTr("?")
                 x: column2x+25; y: -1;  width: 20;
                 onClicked: {
                     calibrationInfo.show();
                }

            }
         }



        CalibrationKeys {}


    }
    // column 3





    Column{
        x:0; y: 520; width: parent.width;
        spacing: columnSpacing;


        Button {
             text: qsTr("Calibrate")
             x: 0; y: 0;  width: 70;

             onClicked: {
                socket.sendAppFocus(projectorSlider.value);
            }

            Text { x: 100; y: 3; color: '#fff'; font.pixelSize: 14; font.family: fontRegular; text: qsTr("mouse to projector #" + (projectorSlider.value+1) ) }

        }


    } // column 4




}////////////////////////////////

