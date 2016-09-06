import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.1

/**************************************************************
 WINDOW SETTINGS
 **************************************************************/
Item {

    id: windowSettings;
    property int column2x: 300;
    property int column3x: column2x+50;
    property var model: model;
    property bool fullReset: false;
    property bool f1: false;
    property bool f2: false;
    property bool f3: false;
    property var projectorCount: 1;

    // tab header
    Text {
        width: 225;
        color: '#fff'; font.pixelSize: fontSizeHeader; font.family: fontBold; font.bold: true;
        text: qsTr("Window")
    }


    // add remove buttons
    Button {
         text: qsTr("Add")
         x: 0; y: 45; width: 65;

         onClicked: {
                 model.append({
                    "index":         list.count,
                    "border":        false,
                    "positionX":     0,
                    "positionY":     0,
                    "pCount":         1,
                    "resolutionX":   1024,
                    "resolutionY":   768

                 })
        }

         Button {
              text: qsTr("Remove")
              x: parent.width+10; y: 0;  width: 65;

              onClicked: {
                if (list.count > 1){
                    model.remove( list.count-1, 1 );
                }
             }
         }
         Button {
              text: qsTr("Apply")
              x: parent.width*2 + 20; y: 0;  width: 65;

              onClicked: {
                  if (fullReset){
                      clearCalibrationWarning.open();
                  }
                  else {
                      saveWindowData();
                  }
             }

         }
    }


    MessageDialog {
        id: clearCalibrationWarning
        title: "Clear Calibration"
        text: "Modifying the projectors will reset all calibration data."
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        onAccepted: {
            saveWindowData();
        }
        Component.onCompleted: visible = false;
    }


    function saveWindowData(){
        var len = 0;
        var ndata = [];
        var ni = 0;

        for (var i=0; i<list.count; i++){
            ndata[ni] = "window"; ni++
            ndata[ni] = list.model.get(i).index; ni++
            ndata[ni] = "border"; ni++
            ndata[ni] = list.model.get(i).border; ni++
            ndata[ni] = "position"; ni++
            ndata[ni] = list.model.get(i).positionX + "," + list.model.get(i).positionY; ni++
            ndata[ni] = "pCount"; ni++
            ndata[ni] = projectorCount; ni++
            ndata[ni] = "resolutionX"; ni++
            ndata[ni] = list.model.get(i).resolutionX; ni++
            ndata[ni] = "resolutionY"; ni++
            ndata[ni] = list.model.get(i).resolutionY; ni++
            ndata[ni] = "fullReset"; ni++
            ndata[ni] = fullReset; ni++
        };



        xml.save(ndata);
        syscmds.restartRenderer();
        fullReset = false;

        settings.projectorSettings.pSlider.maximumValue = parseInt(projectorCount) - 1;
    }



    // list view component
    Component {
        id: delgate


        Column {

            height: 205;
            spacing: columnSpacing;

            // window index
            Text {
                width: 225;
                color: '#fff'; font.pixelSize: fontSizeHeader; font.family: fontBold; font.bold: true;
                text: qsTr("Window " + (index + 1))
            }


            // window border
            Text {
                width: 225;
                color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular
                text: qsTr("Window Border")


                // window border switch
                Switch {
                    x: column2x;
                    checked: border;
                    onCheckedChanged: list.model.get(index).border = checked;
                }
            }




            // window position
            Text {
                width: 400;
                color: '#fff'
                font.pixelSize: fontSizeNormal
                font.family: fontRegular
                text: qsTr("Window Position (x,y) ")


                // x
                TextInput{
                    x: column2x; width: 50;
                    color: '#fff';  font.pixelSize: fontSizeNormal; font.family: fontRegular; horizontalAlignment:  Text.AlignLeft;
                    clip: true; activeFocusOnPress: true; readOnly: false; selectByMouse: true;
                    enabled: true; visible: true;
                    onTextChanged: {
                        list.model.get(index).positionX = parseFloat(text);
                    }
                    Component.onCompleted: {
                        text = list.model.get(index).positionX;
                    }
                }

                // y
                TextInput{
                    x: column3x; y: 0; width: 50;
                    color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular
                    clip: true; activeFocusOnPress: true; readOnly: false;  selectByMouse: true;
                    enabled: true; visible: true;
                    text: "";
                    onTextChanged: {
                        list.model.get(index).positionY = parseFloat(text);
                    }
                    Component.onCompleted: {
                        text = list.model.get(index).positionY;
                    }
                }
            }




            // projector count
            Text {
                id: pCountText
                width: 225;
                color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular
                text: qsTr("Projector Count")


                // input projector count
                TextInput{
                    id: pCountInput;
                    x: column2x;  width: 50;
                    color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular
                    clip: true;  activeFocusOnPress: true; readOnly: false; selectByMouse: true;
                    enabled: true; visible: true;
                    text: pCount;
                    onTextChanged: {
                        if (f1) fullReset = true;
                        f1 = true;
                        projectorCount = parseInt(text);
                    }
                }


            }



            // projector resolution
            Text {
                width: 225;
                color: '#fff'
                font.pixelSize: fontSizeNormal
                font.family: fontRegular
                text: qsTr("Projector Resolution (w,h)")

                // width
                TextInput{
                    x: column2x; y: 0; width: 50;
                    color: '#fff';  font.pixelSize: fontSizeNormal; font.family: fontRegular; horizontalAlignment:  Text.AlignLeft;
                    clip: true; activeFocusOnPress: true; readOnly: false; selectByMouse: true;
                    enabled: true;  visible: true;
                    onTextChanged: {
                        list.model.get(index).resolutionX = parseFloat(text);
                        if (f2) fullReset = true;
                        f2 = true;
                    }
                    Component.onCompleted: {
                        text = list.model.get(index).resolutionX;
                    }
               }

                // height
                TextInput{
                    x: column3x; y: 0; width: 50;
                    color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular
                    clip: true; activeFocusOnPress: true; readOnly: false; selectByMouse: true;
                    enabled: true; visible: true;
                    onTextChanged: {
                        list.model.get(index).resolutionY = parseFloat(text);
                        if (f3) fullReset = true;
                        f3 = true;
                    }
                    Component.onCompleted: {
                        text = list.model.get(index).resolutionY;
                    }
                }

            }



         } // column




    } // component




    ListModel {
        id: model
    }



     ListView {
         id: list;
         y:100; width: 700; height: settings.height
         model: model
         delegate: delgate

          Component.onCompleted: {
                xml.loadSettings();
          }
     }





}
