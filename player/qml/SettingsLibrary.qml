import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.1

/**************************************************************
 LIBRARY SETTINGS
 **************************************************************/
Item {
    property real imageDuration: 10
    property int column2x: 250
    property var dtext: durationText;

    Column {
        x:0; y: 0;
        spacing: columnSpacing;


        Text {
            width: 225;
            color: '#fff'; font.pixelSize: fontSizeHeader; font.family: fontBold; font.bold: true;
            text: qsTr("Library")
        }

        // image duration
        Text {
            width: 200;
            color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular;
            text: qsTr("Image Duration (seconds)")

            TextInput{
                id:durationText
                x: column2x;
                color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular;
                clip: true; activeFocusOnPress: true;  readOnly: false; selectByMouse: true;
                onTextChanged: {
                    imageDuration = text;
                }

            }
        }




        Text {
            width: 200;
            color: '#fff'; font.pixelSize: fontSizeNormal; font.family: fontRegular;
            text: qsTr("Clear library");

            Button {
                 x: column2x; y: -3; width: 50;
                 text: qsTr("Clear");
                 onClicked: {
                     clearWarningDialog.open();
                }
             }
        }




        MessageDialog {
            id: clearWarningDialog
            title: "Clear Library"
            text: "Are you sure you want to clear your playlist and media library?"
            standardButtons: StandardButton.Ok | StandardButton.Cancel
            onAccepted: {
                clearLibrary();
            }
            Component.onCompleted: visible = false;
        }




    } // column




     // clear library database and list models
    function clearLibrary(){
        libraryPanel.clear();
        playlistIndexPanel.clear();
        db.dropLibrary()
        db.dropPlaylist();
        db.dropPlaylistIndex();
        db.createLibrary();
        db.createPlaylist();
        db.createPlaylistIndex();
    }


}
