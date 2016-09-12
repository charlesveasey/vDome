import QtQuick 2.2
import QtGraphicalEffects 1.0

/**************************************************************
 TAB BUTTON
 **************************************************************/
Item {
    id: root

    property alias enabled: mouseArea.enabled
    property bool checkable: false
    property bool checked: false
    property alias hover: mouseArea.containsMouse
    property alias pressed: mouseArea.pressed
    property alias text: text.text
    property alias hit: hit;

    opacity: enabled ? 1.0 : 0.5
    signal clicked

    width: text.width
    height: text.height


    /**************************************************************
     TEXT
     **************************************************************/
    Text {
        id: text
        color: "#ffffff"
        text: text
        font.pixelSize: checked ? 18 : 17;
        font.family: checked ? openSansExtraBold.name : openSansSemiBold.name;

        ColorOverlay {
            id: glowEffect
            anchors.fill: text
            source: text
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
            onClicked: {
                checked = true;
                parent.parent.clicked();
            }

        }

    }

}
