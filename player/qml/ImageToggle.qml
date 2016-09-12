import QtQuick 2.2
import QtGraphicalEffects 1.0

/**************************************************************
 IMAGE TOGGLE BUTTON
 **************************************************************/
Item {
    id: root

    property alias enabled: mouseArea.enabled
    property alias image1Source: image1.source
    property alias image2Source: image2.source

    property bool checkable: false
    property bool checked: false
    property alias hover: mouseArea.containsMouse
    property alias pressed: mouseArea.pressed

    opacity: enabled ? 1.0 : 0.5
    signal clicked

    width: image1.width
    height: image1.height

    /**************************************************************
     IMAGE 1
     **************************************************************/
    Image {
        id: image1
        anchors.verticalCenter: parent.verticalCenter
        visible: true
    }

    /**************************************************************
     IMAGE 2
     **************************************************************/
    Image {
        id: image2
        anchors.verticalCenter: parent.verticalCenter
        visible: true
    }

    /**************************************************************
     HOVER EFFECT
     **************************************************************/
    ColorOverlay {
        id: glowEffect
        anchors.fill: image
        source: image
        color: pressed ? "#22000000" : checked ? "orange" : "white"
        visible: checked || hover || pressed
    }

    /**************************************************************
     MOUSE AREA
     **************************************************************/
    MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: parent
        onClicked: parent.clicked();
    }
}

