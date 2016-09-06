import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.1
import QtQuick.Controls.Styles 1.2

/**************************************************************
 SETTING WINDOW
 **************************************************************/
Window {
    id: root
    title: ""
    width: window.width/2; height: window.height
    minimumWidth: 400; minimumHeight: 265;
    color: '#111'; opacity: 1
    flags: Qt.Window | Qt.WindowTitleHint | Qt.WindowCloseButtonHint | Qt.WindowSystemMenuHint

    property int  sliderInputTextPad: 100
    property var settingsObject;

    property int fontSizeHeader: 18;
    property int fontSizeNormal: 16;
    property int columnSpacing: 16;

    property string fontRegular: openSansSemiBold.name;
    property string fontBold: openSansExtraBold.name;

    property var windowSettings: settingsWindow;
    property var projectorSettings: settingsProjector;

    Component.onCompleted: {
        db.createSettings();
        load();
    }

    Component.onDestruction: {

        // save settings objects
        settingsObject.librarySlide = settingsLibrary.imageDuration;
        settingsObject.socketHost = socket.host;
        settingsObject.socketSend = socket.sendPort;
        settingsObject.socketReceive = socket.receivePort;
        db.updateSettings(settingsObject);
        libraryPanel.slide = settingsObject.librarySlide;
        libraryPanel.updateDuration();
        libraryPanel.loadSort();

        settingsLibrary.clearLibrary();
    }


    Row{
        x: 20; y: 15;
        spacing: 30

        /**************************************************************
         TRANSFORM SETTINGS TAB
         **************************************************************/
        TabButton {
            id: settingsInputTab;
            y: checked ? 0 : 2;
            text: qsTr("Input")
            hit.width: 65; hit.height: 50; hit.x: -5; hit.y: -25;
            checked: false;
            onClicked: {
                settingsProjectorTab.checked = false;
                settingsWindowTab.checked = false;
                settingsLibraryTab.checked = false;
                settingsSocketTab.checked = false;
            }
        }

        /**************************************************************
         PROJECTOR SETTINGS TAB
         **************************************************************/
        TabButton {
            id: settingsProjectorTab;
            y: checked ? 0 : 2;
            text: qsTr("Projector")
            hit.width: 65; hit.height: 50; hit.x: -5; hit.y: -25;
            checked: false;
            onClicked: {
                settingsInputTab.checked = false;
                settingsWindowTab.checked = false;
                settingsLibraryTab.checked = false;
                settingsSocketTab.checked = false;
            }
        }

        /**************************************************************
         WINDOW SETTINGS TAB
         **************************************************************/
        TabButton {
            id: settingsWindowTab;
            y: checked ? 0 : 2;
            text: qsTr("Window")
            hit.width: 65; hit.height: 50; hit.x: -5; hit.y: -25;
            checked: false;
            onClicked: {
                settingsInputTab.checked = false;
                settingsProjectorTab.checked = false;
                settingsLibraryTab.checked = false;
                settingsSocketTab.checked = false;
            }
        }

        /**************************************************************
         LIBRARY SETTINGS TAB
         **************************************************************/
        TabButton {
            id: settingsLibraryTab;
            y: checked ? 0 : 2;
            text: qsTr("Libary")
            hit.width: 65; hit.height: 50; hit.x: -5; hit.y: -25;
            checked: false;
            onClicked: {
                settingsProjectorTab.checked = false;
                settingsWindowTab.checked = false;
                settingsInputTab.checked = false;
                settingsSocketTab.checked = false;
            }
        }

        /**************************************************************
         SOCKET SETTINGS TAB
         **************************************************************/
        TabButton {
            id: settingsSocketTab;
            y: checked ? 0 : 2;
            text: qsTr("Socket")
            hit.width: 65; hit.height: 50; hit.x: -5; hit.y: -25;
            checked: false;
            onClicked: {
                settingsInputTab.checked = false;
                settingsWindowTab.checked = false;
                settingsProjectorTab.checked = false;
                settingsLibraryTab.checked = false;
            }
        }

    }

    /**************************************************************
     SETTINGS TAB CONTAINERS
     **************************************************************/
    Rectangle{
        x: 0;  y: 55; width: parent.width; height: parent.height-y*2;
        color: '#212121'

        SettingsInput{
             id: settingsInput;
             visible: settingsInputTab.checked;
             x: 20; y: 20; z: 5;
        }
        SettingsProjector{
            id: settingsProjector;
            visible: settingsProjectorTab.checked;
            x: 20; y: 20; z: 5;
        }
        SettingsWindow{
            id: settingsWindow;
            visible: settingsWindowTab.checked;
            x: 20; y: 20;  z: 5;
        }
        SettingsLibrary{
             id: settingsLibrary;
             visible: settingsLibraryTab.checked;
             x: 20; y: 20; z: 5;
        }
        SettingsSocket{
            id: settingsSocket;
            visible: settingsSocketTab.checked;
            x: 20; y: 20; z: 5;
        }
    }


    /**************************************************************
     LOAD SETTINGS FROM DB
     **************************************************************/
    function load(){
        settingsObject = {};
        var rows = db.selectSettings();

        var o = {};
        if (rows.length){
            o = rows[0];

            libraryPanel.slide = o.librarySlide;
            libraryPanel.updateDuration();
            settingsLibrary.imageDuration = o.librarySlide;

            settingsLibrary.dtext.text = o.librarySlide;

            socket.host = o.socketHost;
            socket.sendPort = parseInt(o.socketSend);
            socket.receivePort = parseInt(o.socketReceive);

            settingsSocket.hText.text = socket.host;
            settingsSocket.sText.text = socket.sendPort;
            settingsSocket.rText.text = socket.receivePort;

            for (var p in  o){
                settingsObject[p] = o[p]
            }
        }
    }

    /**************************************************************
     LOAD SETTINGS FROM DB
     **************************************************************/
    function open(tab){
        if (tab === 'input'){
            settingsInputTab.checked = true;
            settingsLibraryTab.checked = false;
            settingsSocketTab.checked = false;
            settingsProjectorTab.checked = false;
            settingsWindowTab.checked = false;
        }
        else if (tab === 'projector'){
            settingsInputTab.checked = false;
            settingsLibraryTab.checked = false;
            settingsSocketTab.checked = false;
            settingsProjectorTab.checked = true;
            settingsWindowTab.checked = false;
        }
        else if (tab === 'window'){
            settingsInputTab.checked = false;
            settingsLibraryTab.checked = false;
            settingsSocketTab.checked = false;
            settingsProjectorTab.checked = false;
            settingsWindowTab.checked = true;
        }
        else if (tab === 'library'){
            settingsInputTab.checked = false;
            settingsLibraryTab.checked = true;
            settingsSocketTab.checked = false;
            settingsProjectorTab.checked = false;
            settingsWindowTab.checked = false;
        }
        else if (tab === 'socket'){
            settingsInputTab.checked = false;
            settingsLibraryTab.checked = false;
            settingsSocketTab.checked = true;
            settingsProjectorTab.checked = false;
            settingsWindowTab.checked = false;
        }
        visible = true;
    }


}////////////////////////////////////////////
