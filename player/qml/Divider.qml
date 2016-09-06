import QtQuick 2.2

/**************************************************************
 DIVIDER
 **************************************************************/
Image {
    property real defaultX: parent.width-.25*parent.width;
    property real cachedX: defaultX;
    property real cachedW: root.parent.width;
    property real mouseDeltaX: 0;

    id: root;
    x: defaultX; y: 124; source: "../images/divider.png"; height:parent.height-140
    opacity: x <= 150 ? x/150 :
                        x > parent.width-150 ? nMap(x, parent.width-150, parent.width, 1, 0) : 1

    /**************************************************************
     HIT AREA
     **************************************************************/
    Rectangle {
        color: "white";
        width: parent.width * 10;
        height: parent.height;
        x: -parent.width;
        opacity: 0;

        /**************************************************************
         MOUSE AREA
         **************************************************************/
        MouseArea {
            id: mouseArea
            width: parent.width;
            height: parent.height;
            property real mousePressedX: 0;

            onPressed: {
                mousePressedX = mouseX;
            }

            onMouseXChanged: {
                if (pressed) {
                    mouseDeltaX = mouseX - mousePressedX;
                    if (mouseDeltaX < 0 && root.x+mouseDeltaX > -1)
                        root.x += mouseDeltaX;
                    else if (mouseDeltaX >= 0  && root.x+mouseDeltaX <= root.parent.width)
                        root.x += mouseDeltaX;
                    root.cachedX = root.x;
                    root.cachedW = root.parent.width;
                }


            }
        }
    }

}
