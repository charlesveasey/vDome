import QtQuick 2.0;

/**************************************************************
 SCROLL BAR
 **************************************************************/
Item {
    id: scrollbar;
    width: (handleSize + 2 * (backScrollbar.border.width +1));
    visible: (flickable.visibleArea.heightRatio < 1.0);
    anchors {
        top: flickable.top;
        right: flickable.right;
        bottom: flickable.bottom;
        margins: 1;
    }

    property Flickable flickable               : null;
    property int       handleSize              : 10;


    /**************************************************************
     BACKGROUND
     **************************************************************/
    Rectangle {
        id: backScrollbar;
        radius: 5;
        antialiasing: true;
        color: "#333"
        border {
            width: 0;
            color: "darkgray";
        }
        anchors { fill: parent; }

        MouseArea {
            anchors.fill: parent;
            onClicked: { }
        }
    }


    /**************************************************************
     HANDLE
     **************************************************************/
    Item {
        id: groove;
        clip: true;
        anchors {
            fill: parent;
            topMargin: (backScrollbar.border.width +1 );
            leftMargin: (backScrollbar.border.width +1);
            rightMargin: (backScrollbar.border.width +1);
            bottomMargin: (backScrollbar.border.width +1);
        }

        MouseArea {
            id: clicker;
            drag {
                target: handle;
                minimumY: 0;
                maximumY: (groove.height - handle.height);
                axis: Drag.YAxis;
            }
            anchors { fill: parent; }
            onClicked: { flickable.contentY = (mouse.y / groove.height * (flickable.contentHeight - flickable.height)); }
        }
        Item {
            id: handle;
            height: Math.max (20, (flickable.visibleArea.heightRatio * groove.height));
            anchors {
                left: parent.left;
                right: parent.right;
            }

            Rectangle {
                id: backHandle;
                color: (clicker.pressed ? "0x444" : "0xfff");
                opacity: (flickable.moving ? 0.65 : 0.35);
                anchors { fill: parent; }
                radius: 5;

                Behavior on opacity { NumberAnimation { duration: 150; } }
            }
        }
    }

    Binding {
         target: handle;
         property: "y";
         value: (flickable.contentY * clicker.drag.maximumY / (flickable.contentHeight - flickable.height));
         when: (!clicker.drag.active);
    }

    Binding {
        target: flickable;
        property: "contentY";
        value: (handle.y * (flickable.contentHeight - flickable.height) / clicker.drag.maximumY);
        when: (clicker.drag.active || clicker.pressed);
    }


    /**************************************************************
     SCROLL DOWN
     **************************************************************/
    function scrollDown () {
        flickable.contentY = Math.min (flickable.contentY + (flickable.height / 4), flickable.contentHeight - flickable.height);
    }


    /**************************************************************
     SCROLL UP
     **************************************************************/
    function scrollUp () {
        flickable.contentY = Math.max (flickable.contentY - (flickable.height / 4), 0);
    }

}
