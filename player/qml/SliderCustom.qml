import QtQuick 2.0

/**************************************************************
 CUSTOM SLIDER
 **************************************************************/
Item {
    id: slider

    height: background.height
    // value is read/write.
    property real value: 0
    property real maximum: 1
    property real minimum: 0
    property int xMax: width - handle.width
    onXMaxChanged: updatePos()
    onMinimumChanged: updatePos()
    onValueChanged: if (!pressed) updatePos()
    property bool mutable: true
    property alias pressed : backgroundMouse.pressed
    property alias hit: hit

    signal valueChangedByHandle(int newValue)


    /**************************************************************
     BACKGROUND
     **************************************************************/
    BorderImage {
        id: background
        source: "../images/slider-background.png"
        width: slider.width
        border.left: 6
        border.right: 6

        Rectangle {
            id: hit
            width: parent.width; height: parent.height;
            x: 0; y: 0;
            color: "red"
            opacity: 0.0

            MouseArea {
                id: backgroundMouse
                anchors.fill: parent
                enabled: slider.mutable
                drag.target: handle
                drag.axis: Drag.XAxis
                drag.minimumX: 0
                drag.maximumX: slider.xMax
                onReleased: {
                    value = Math.max(minimum, Math.min(maximum, (maximum - minimum) * (mouseX - handle.width/2) / slider.xMax + minimum));
                    valueChangedByHandle(value);
                    updatePos();
                }
                onPositionChanged: {
                    value = Math.max(minimum, Math.min(maximum, (maximum - minimum) * (mouseX - handle.width/2) / slider.xMax + minimum));
                    valueChangedByHandle(value);
                    updatePos();
                }
                /*
                onWheel: {
                    value = Math.max(minimum, Math.min(maximum, value + (wheel.angleDelta.y > 0 ? 1 : -1) * (10 / slider.xMax) * (slider.maximum - slider.minimum)));
                    valueChangedByHandle(value);
                    updatePos();
                }
                */
            }
        }
    }


    /**************************************************************
     PROGRESS
     **************************************************************/
    BorderImage {
        id: progress
        source: "../images/slider-progress.png"
        anchors.topMargin: 1
        anchors.top: parent.top
        anchors.bottomMargin: 17
        anchors.bottom: parent.bottom
        anchors.left: background.left
        anchors.right: handle.right
        anchors.rightMargin: 2
        border.left: 7
        border.right: 7
        visible: slider.enabled
    }


    /**************************************************************
     HANDLE
     **************************************************************/
    BorderImage {
        id: handle
        source: "../images/slider-handle.png"
        border.left: 7
        border.right: 7
        anchors.verticalCenter: background.verticalCenter
        visible: slider.enabled
        opacity: slider.mutable ? backgroundMouse.pressed ? 1 : 0.8 : 0.5
    }


    /**************************************************************
     UPDATE POSITION
     **************************************************************/
    function updatePos() {
        if (maximum > minimum) {
            var pos = 0 + (value - minimum) * slider.xMax / (maximum - minimum);
            pos = Math.min(pos, width - handle.width - 0);
            pos = Math.max(pos, 0);
            handle.x = pos;
        } else {
            handle.x = 0;
        }
    }

}

