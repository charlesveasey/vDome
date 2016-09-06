import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.0
import QtQuick.Controls 1.2
import Playlist 1.0
import Syscmds 1.0
import XMLParser 1.0

/**************************************************************
 WINDOW
 **************************************************************/
ApplicationWindow {
    property int defaultW: 1200; property int defaultH: 700
    property int minW: 1000;  property int minH: 100

    id: window
    width: 1200; height: 700;
    minimumWidth: minW; minimumHeight: minH;
    visible: true;
    title: ""
    //flags: Qt.WindowActive
    //flags: Qt.FramelessWindowHint


    onClosing: {
        syscmds.closeRenderer();
    }

    onWidthChanged: {
        divider.x = nMap(divider.cachedX, 0, divider.cachedW, 0, width)
    }


    /**************************************************************
     SYSTEM SETTINGS
     **************************************************************/
    Settings {
        id: settings;
    }


    /**************************************************************
     SYSTEM SETTINGS
     **************************************************************/
    XMLParser {
        id: xml;
        function loadSettings() {
            var xmlData = xml.load();
            var windowFlag = false;

            var index, border, position, positionX, positionY, pCount, resolution, resolutionX, resolutionY;
            var s = [];

            for (var i= 0; i<xmlData.length; i++){

                if (xmlData[i] === "window"){       // window
                    if (windowFlag){
                        settings.windowSettings.model.append({
                                 "index":           index,
                                 "border":          border,
                                 "positionX":       positionX,
                                 "positionY":       positionY,
                                 "pCount":           pCount,
                                 "resolutionX":     resolutionX,
                                 "resolutionY":     resolutionY
                                 });
                    }

                    // index
                    index = (parseInt(xmlData[i+1]));

                    // border
                    border = (xmlData[i+3] === "on") ? true : false;

                    // position
                    position = xmlData[i+5];
                    s = position.split(",");
                    positionX = parseInt(s[0]);
                    positionY = parseInt(s[1]);

                    // projectors
                    pCount = (parseInt(xmlData[i+7]));

                    // resolution
                    resolution = xmlData[i+9];
                    s = resolution.split(",");
                    resolutionX = parseInt(s[0]);
                    resolutionY = parseInt(s[1]);

                    // set window flag
                    windowFlag = true;
                }
            }

            settings.windowSettings.model.append({
                 "index":           index,
                 "border":          border,
                 "positionX":       positionX,
                 "positionY":       positionY,
                 "pCount":          pCount,
                 "resolutionX":     resolutionX,
                 "resolutionY":     resolutionY
            });

           settings.projectorSettings.pSlider.maximumValue = pCount - 1;
        }

    }



    /**************************************************************
     SYSTEM MENU
     **************************************************************/
    menuBar: sysmenu
    SystemMenu{ id:sysmenu }


    CalibrationInfo{
        id:calibrationInfo;
    }


    /**************************************************************
     FONTS
     **************************************************************/
    FontLoader { id: openSansRegular; source: "../fonts/OpenSans-Regular.ttf" }
    FontLoader { id: openSansItalic; source: "../fonts/OpenSans-Italic.ttf" }
    FontLoader { id: openSansExtraBold; source: "../fonts/OpenSans-ExtraBold.ttf" }
    FontLoader { id: openSansSemiBold; source: "../fonts/OpenSans-Semibold.ttf" }

    /**************************************************************
     SOCKET
     **************************************************************/
    Socket {
        id: socket
        host: "localhost";
        sendPort: 3334;
        receivePort: 3333;
    }

    /**************************************************************
     DATABASE
     **************************************************************/
    Database {
        id: db
    }

    /**************************************************************
     PLAYLIST FILES I/O
     **************************************************************/
    Playlist {
        id: plist
    }

    /**************************************************************
     SYSTEM COMMANDS
     **************************************************************/
    Syscmds {
        id: syscmds

        Component.onCompleted: {
            syscmds.startRenderer();
        }
    }

    /**************************************************************
     BACKGROUND
     **************************************************************/
    Rectangle {
        color:"black"
        width:parent.width; height:parent.height;
    }

    /**************************************************************
     CONTROL PANEL
     **************************************************************/
    ControlPanel {
        id: controlPanel
        z: 10;
        onSourcePressed: {
            globalMouseArea.enabled = false;
        }
        onSelectionPopup: {
            globalMouseArea.enabled = true;
        }
    }

    /**************************************************************
     PLAYLIST PANEL
     **************************************************************/
    PlaylistPanel {
        id: playlistPanel;
        onEditBegan: {
            libraryPanel.setInteractive(false);
        }
        onEditEnded: {
            libraryPanel.setInteractive(true);
            controlPanel.focus = true;
        }
        onMousePressed: {
            libraryPanel.setCurrentIndex(-1);
            libraryPanel.clearHeld();
            playlistIndexPanel.setCurrentIndex(-1);
        }
    }

    /**************************************************************
     DIVIDER
     **************************************************************/
    Divider {
        id: divider
    }

    /**************************************************************
     LIBRARY PANEL
     **************************************************************/
    LibraryPanel {
        id: libraryPanel;

        onEditBegan: {
            playlistPanel.setInteractive(false);
        }
        onEditEnded: {
            playlistPanel.setInteractive(true);
            controlPanel.focus = true;
        }
        onMousePressed: {
            playlistPanel.setCurrentIndex(-1);
            controlPanel.forceActiveFocus();
        }
    }

    /**************************************************************
     PLAYLIST INDEX PANEL
     **************************************************************/
    PlaylistIndexPanel {
        id:playlistIndexPanel
        onEditBegan: {
            playlistPanel.setInteractive(false);
        }
        onEditEnded: {
            playlistPanel.setInteractive(true);
            controlPanel.focus = true;
        }
        onMousePressed: {
            playlistPanel.setCurrentIndex(-1);
        }
    }

    /**************************************************************
     GLOBAL MOUSE
     **************************************************************/
    MouseArea {
        id: globalMouseArea
        width:parent.width; height: parent.height;
        enabled: false;
        //z: 100;
        onPressed: {
            controlPanel.closeSelectionPopup();
            globalMouseArea.enabled = false;
        }
    }

    /**************************************************************
     UTIL FUNCTIONS
     **************************************************************/
    function formatURL(filepath) {
        var index = filepath.lastIndexOf('/')
        return filepath.substr(index+1)
    }

    function hmsToSeconds(hms){
        var a = hms.split(':');

        if (!a[0]) a[0] = 0;
        if (!a[1]) a[1] = 0;
        if (!a[2]) a[2] = 0;

        return (+a[0]) * 60 * 60 + (+a[1]) * 60 + (+a[2]);
    }

    function secondsToHms(d) {

        if (d === null)
            return "00:00:00";

        d = Number(d);
        var h = Math.floor(d / 3600);
        var m = Math.floor(d % 3600 / 60);
        var s = Math.floor(d % 3600 % 60);
        var hs = "";
        var ms = "";
        var ss = "";
        if (h < 10) hs = "0" + h;
        else        hs = h;
        if (m < 10) ms = "0" + m;
        else        ms = m;
        if (s < 10) ss = "0" + s;
        else        ss = s;

        return hs + ":" + ms + ":" + ss;
    }

    function nMap(x, a1, a2, b1, b2) {
        return ((x - a1)/(a2 - a1)) * (b2 - b1) + b1;
    }

}


