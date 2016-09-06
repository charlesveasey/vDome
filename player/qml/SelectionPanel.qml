import QtQuick 2.0

/**************************************************************
 SELECTION PANEL
 **************************************************************/
Item {
    id: root

    property int itemHeight: 25
    property string selectedItem: ""
    property bool isMouseAbove: mouseAboveMonitor.containsMouse
    property alias model: sources;
    property alias list: list;


    signal selectionPopup;
    signal exited;

    Keys.onEscapePressed: {
        if (root.visible){
            root.visible = false
            root.exited();
        }
    }

    signal clicked
    QtObject {
        id: d
        property Item selectedItem
    }


    /**************************************************************
     DATA MODEL
     **************************************************************/
    ListModel {
        id: sources
    }


    /**************************************************************
     MOUSE AREA
     **************************************************************/
    MouseArea {
        id: mouseAboveMonitor
        anchors.fill: parent
        hoverEnabled: true
    }

    Rectangle {
        color: '#333333';
        opacity: .8;
        width: parent.width
        height: parent.height
        radius: 10
    }

    /**************************************************************
     POPUP
     **************************************************************/
    ListView {
        id: list
        anchors.fill: parent
        clip: true
        anchors.margins: 14
        model: sources
        focus: root.visible && root.opacity
        currentIndex: 0
        property variant myData: sources

        onVisibleChanged: {
            if (visible) root.selectionPopup();
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true

            onMouseYChanged: {
                list.currentIndex = list.indexAt(mouseX,mouseY);
            }
            onClicked: {
                selectedItem = sources.get(list.currentIndex).name;
                root.clicked();
            }
        }

        delegate: Item {
            height: 23
            width: parent.width
            Rectangle {
                anchors.fill: parent
                //border.color: index == list.currentIndex ? "#44ffffff" : "transparent"
                color: index == list.currentIndex ? "#22ffffff" : "transparent"
                radius: 6
                Text {
                    color: "#ffffff"
                    font.pixelSize: 13
                    font.family: openSansSemiBold.name
                    font.bold: true;
                    text: name ; anchors.centerIn: parent}
            }
            MouseArea {
                anchors.fill: parent
                onClicked:  {
                    list.currentIndex = index
                }
            }
        }
    }

    Component {
        id: sourceDelegate
        Item {
            id: sourceDelegateItem
            width: root.width
            height: itemHeight

            Text {
                id: sourceSelectorItem
                anchors.centerIn: parent
                width: 0.9 * parent.width
                height: 0.8 * itemHeight
                text: name
                color: "white"
            }

            states: [
                State {
                    name: "selected"
                    PropertyChanges {
                        target: sourceSelectorItem
                        //bgColor: "#ff8888"
                    }
                }
            ]

            transitions: [
                Transition {
                    from: "*"
                    to: "*"
                    ColorAnimation {
                        properties: "color"
                        easing.type: Easing.OutQuart
                        duration: 500
                    }
                }
            ]
        }
    }

}
