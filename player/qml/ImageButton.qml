import QtQuick 2.2
import QtGraphicalEffects 1.0

/**************************************************************
 IMAGE BUTTON
 **************************************************************/
Item {
    id: root

    property alias enabled: mouseArea.enabled
    property alias imageSource: image.source
    property bool checkable: false
    property bool checked: false
    property alias hover: mouseArea.containsMouse
    property alias pressed: mouseArea.pressed
    property alias hit: hit;

    opacity: enabled ? 1.0 : 0.5
    signal clicked

    width: image.width
    height: image.height

    /**************************************************************
     IMAGE
     **************************************************************/
    Image {
        id: image
        anchors.verticalCenter: parent.verticalCenter
        visible: true

        ColorOverlay {
            id: glowEffect
            anchors.fill: image
            source: image
            color: pressed ? "#22000000" : checked ? "white" : "white"
            visible: checked || hover || pressed
        }
    }

    /**************************************************************
     HIT AREA
     **************************************************************/
    Rectangle {
        id: hit
        width: parent.width; height: parent.height;
        x: 0; y: 0;
        color: "red"
        opacity: 0.0

        MouseArea {
            id: mouseArea
            hoverEnabled: true
            anchors.fill: parent
            onClicked: parent.parent.clicked();
        }

    }

}
