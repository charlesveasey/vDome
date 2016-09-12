import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.0
import QtQuick.Controls 1.2

/**************************************************************
 SYSTEM MENU
 **************************************************************/
MenuBar {
    id: sysmenu

    /**************************************************************
     PLAYLIST MENU
     **************************************************************/
    Menu {
        title: "Playlist"

        MenuItem {
            text: "Add"
            onTriggered: playlistPanel.add();
        }

        MenuItem {
            text: "Remove"
            onTriggered: playlistPanel.remove();
        }

        MenuSeparator { }

        MenuItem {
            text: "Save"
            onTriggered: playlistPanel.save();

        }
        MenuItem {
            text: "Clear"
            onTriggered: playlistPanel.clear();

        }

        MenuSeparator { }

        MenuItem {
            text: "Import"
            onTriggered: playlistPanel.importP();

        }
        MenuItem {
            text: "Export"
            onTriggered: playlistPanel.exportP();
        }
    }

    /**************************************************************
     LIBRARY MENU
     **************************************************************/
    Menu {
        title: "Library"
        MenuItem {
            text: "Add"
            onTriggered: libraryPanel.add();
        }
        MenuItem {
            text: "Remove"
            onTriggered: {
                if (controlPanel.currentPanel == libraryPanel || controlPanel.currentPanel == playlistIndexPanel)
                    controlPanel.currentPanel.remove();
            }
        }
    }

    /**************************************************************
     VIEW MENU
     **************************************************************/
    Menu {
        title: "View"
        MenuItem {
            text: "Default"
            onTriggered: {
                divider.x = divider.defaultX;
                playlistPanel.visible = true;
                libraryPanel.visible = true;
            }
        }
        MenuItem {
            id: viewPlaylistMenuItem
            text: "Playlist"
            checkable: true
            checked: playlistPanel.visible
            onTriggered: {
                if (checked && viewLibraryMenuItem.checked){
                    window.height = window.defaultH;
                    divider.x = divider.defaultX;
                }
                else if (checked && !viewLibraryMenuItem.checked){
                    window.height = window.defaultH;
                    divider.x = window.width;
                }
                else if (!checked && !viewLibraryMenuItem.checked){
                    window.height = window.minH;
                }
                else if (!checked && viewLibraryMenuItem.checked){
                    window.height = window.defaultH;
                    divider.x = 0;
                }
                divider.cachedX = divider.x;
            }
        }
        MenuItem {
            id: viewLibraryMenuItem
            text: "Library"
            checkable: true
            checked: libraryPanel.visible
            onTriggered: {
                if (checked && viewPlaylistMenuItem.checked){
                    window.height = window.defaultH;
                    divider.x = divider.defaultX;
                }
                else if (checked  && !viewPlaylistMenuItem.checked){
                    window.height = window.defaultH;
                    divider.x = 0;
                }
                else if (!checked && !viewPlaylistMenuItem.checked){
                    window.height = window.minH;
                }
                else if (!checked  && viewPlaylistMenuItem.checked){
                    window.height = window.defaultH;
                    divider.x = window.width;
                }
                divider.cachedX = divider.x;
            }
        }
    }


    /**************************************************************
     SETTINGS MENU
     **************************************************************/
    Menu {
        title: "Setup"
        MenuItem {
            text: "Input"
            onTriggered: settings.open("input");
        }
        MenuItem {
            text: "Projector"
            onTriggered: settings.open("projector");
        }
        MenuItem {
            text: "Window"
            onTriggered: settings.open("window");
        }
        MenuItem {
            text: "Library"
            onTriggered: settings.open("library");
        }
        MenuItem {
            text: "Socket"
            onTriggered: settings.open("socket");
        }
    }

}///////////////////////////////////////////
