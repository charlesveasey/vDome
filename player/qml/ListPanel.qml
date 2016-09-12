import QtQuick 2.0

/**************************************************************
 LIST PANEL
 **************************************************************/
Item {
    id: root;
    property alias list: list;
    property alias mouseArea:list.mouseArea;
    property alias model:list.model;
    property alias dragIconVisible: dragIcon.visible;
    property alias authorHeaderX: authorHeader.x;
    property alias authorHeaderVisible: authorHeader.visible;
    property alias durationHeaderX: durationHeader.x;
    property alias durationHeaderVisible: durationHeader.visible;

    property int activeIndex: -1;
    property bool reorder: false;
    property bool allowDrag: false;
    property bool allowSort: true;
    property bool durationEditable: false;
    property bool groupSelectionActive: false;
    property bool groupShiftActive: false;
    property bool groupSelectionEnabled: false;
    property string titleHeaderText: "Title";

    signal mouseReleased;
    signal mousePressed;
    signal mouseDoubleClicked;
    signal mousePositionChanged;
    signal listComplete;
    signal editBegan;
    signal editEnded;
    signal indexChanged;
    signal completed;
    signal fileDragged(variant files);
    signal titleClicked();
    signal authorClicked()
    signal durationClicked();

    /**************************************************************
     LIST HEADERS
     **************************************************************/
    TextButton {
        id: titleHeader
        x: 20; y: -10;
        text: titleHeaderText
        font.family: openSansExtraBold.name;
        font.pixelSize: 15
        color: "#ffffff"
        enabled: root.allowSort;
        onClicked: root.titleClicked();
    }
    TextButton {
        id: authorHeader
        x: parent.width-parent.width*.4; y: titleHeader.y;
        text: qsTr("Author")
        font.family: openSansExtraBold.name;
        font.pixelSize: 15
        visible: parent.width > 420;
        color: "#ffffff"
        enabled: root.allowSort;
        onClicked: root.authorClicked();
    }
    TextButton {
        id: durationHeader
        y: titleHeader.y;
        anchors.right: parent.right;
        width: 85;
        color: "#ffffff"
        text: qsTr("Duration")
        font.family: openSansExtraBold.name;
        font.pixelSize: 15
        enabled: root.allowSort;
        visible: parent.width > 300;
        onClicked: root.durationClicked();
    }

    /**************************************************************
     DRAG ICON
     **************************************************************/
    Image {
        id: dragIcon
        visible: false
        source: "../images/drag-icon.png";
        z:100;
        opacity: .5
    }


    /**************************************************************
     HIGHLIGHT
     **************************************************************/
    Component {
         id: highlightDelegate
         Item {
             width: parent == null ? 0 : parent.width
             z: 10
             visible: !groupSelectionEnabled;
             Rectangle {
                 width: list.width-1
                 height: 22
                 color: "#FFF"
                 border.width: 0
                 border.color: "#FFF"
                 radius: 5
                 opacity: .125
                 y: 2;
             }
         }
     }


    /**************************************************************
     DATA MODEL
     **************************************************************/
    ListModel {
        id: dataModel
        Component.onCompleted: {
            root.completed();
        }
    }


    /**************************************************************
     LIST VIEW
     **************************************************************/
    ListView {
        id: list
        anchors.fill: parent
        clip: true
        anchors.margins: 14
        model: dataModel
        highlightMoveDuration: 1;
        currentIndex: list.currentIndex;
        spacing: 0;
        snapMode: ListView.SnapToItem;

        highlight:highlightDelegate;
        highlightFollowsCurrentItem: !groupSelectionEnabled;

        property variant myData: dataModel
        property variant mouseArea: mouseArea


        /**************************************************************
         REORDER INDICATOR
         **************************************************************/
        Rectangle {
            id:reorderIndicator;
            width: root.width-35;
            opacity: 1;
            color: '#444444'
            height: 3;
            radius: 5;
            z: 10;
            visible:false;
            x: 3;
         }


        delegate:

            Item {
                id: itemDelegate
                height: 24
                width: parent.width-10
                Drag.active: !allowDrag ? false : activeIndex >=0 ? dataModel.get(activeIndex).held : -1
                property alias tTex:titleText;
                property alias aTex:authorText;
                property alias dTex:durationText;




                Rectangle {
                    id: background
                    anchors.fill: parent
                    color: "#000"
                    radius: 0




                    Item {
                        width: parent == null ? 0 : parent.width
                        z: 10
                        visible: held && groupSelectionEnabled;
                        Rectangle {
                            width: list.width-1
                            height: 22
                            color: "#FFF"
                            border.width: 0
                            border.color: "#FFF"
                            radius: 5
                            opacity: 0.125
                            y: 2;
                        }
                    }

                    TextInput{
                        id:titleText
                        x:7;
                        y:4.5;
                        color: "white";
                        width: authorHeader.visible ? parent.width * .6 - 15 :
                                                      durationHeader.visible ? parent.width - durationText.width :
                                                      parent.width;
                        text: title == null || title == "" ? "----" : title;
                        font.pixelSize: 12
                        font.family: openSansSemiBold.name;
                        z: 0
                        activeFocusOnPress: false
                        cursorVisible: false
                        opacity: 1;
                        readOnly: false;
                        selectByMouse: true;
                        onAccepted: {
                            dataModel.setProperty(activeIndex, "title", displayText)
                            cursorVisible = false;
                            mouseArea.z = 1;
                            list.interactive = true;
                            root.editEnded();
                        }
                        visible: cursorVisible

                    }
                    Text{
                        id:titleTextT
                        x:titleText.x
                        y:titleText.y
                        color: titleText.color
                        width: titleText.width
                        text: titleText.text;
                        font.pixelSize: titleText.font.pixelSize
                        font.family: titleText.font.family
                        z: titleText.z
                        elide: Text.ElideRight
                        visible: !titleText.cursorVisible
                    }
                    Item{
                        id: aligner;
                        x: authorHeader.x-15
                    }
                    TextInput{
                        id:authorText
                        y:4.5;
                        anchors.left: aligner.left;
                        anchors.right: durationText.left;
                        anchors.rightMargin: 15;
                        color: "white";
                        text: author == null || author == "" ? "----" : author;
                        font.pixelSize: 12
                        font.family: openSansSemiBold.name;
                        font.bold: false;
                        z: 0
                        clip: true;
                        activeFocusOnPress: false
                        cursorVisible: false
                        opacity: 1;
                        readOnly: false;
                        selectByMouse: true;
                        onAccepted: {
                            dataModel.setProperty(activeIndex, "author", displayText)
                            cursorVisible = false;
                            mouseArea.z = 1;
                            list.interactive = true;
                            root.editEnded();
                        }
                        visible: authorHeader.visible && cursorVisible
                    }
                    Text{
                        id:authorTextT
                        x:authorText.x
                        y:authorText.y
                        color: authorText.color
                        width: authorText.width
                        text: authorText.text;
                        font.pixelSize: authorText.font.pixelSize
                        font.family: authorText.font.family
                        z: authorText.z
                        elide: Text.ElideRight
                        visible: authorHeader.visible && !authorText.cursorVisible
                    }
                    TextInput{
                        id:durationText
                        x: durationHeader.x-14; y:4.5; color: "white";
                        text: secondsToHms(duration);
                        font.pixelSize: 12
                        font.family: openSansSemiBold.name;
                        font.bold: false;
                        clip: true;
                        activeFocusOnPress: false
                        cursorVisible: false
                        opacity: 1;
                        readOnly: false;
                        selectByMouse: true;
                        enabled: durationEditable;
                        onAccepted: {
                            dataModel.setProperty(activeIndex, "duration", hmsToSeconds(displayText))
                            cursorVisible = false;
                            mouseArea.z = 1;
                            list.interactive = true;
                            root.editEnded();
                        }
                        visible: durationHeader.visible && cursorVisible
                    }
                    Text {
                        id: durationTextT;
                        x: durationText.x; y:durationText.y; color: durationText.color;
                        text: durationText.text
                        width: durationText.width
                        font.pixelSize: durationText.font.pixelSize;
                        font.family: durationText.font.family;
                        font.bold: durationText.font.bold;
                        visible: durationHeader.visible && !durationText.cursorVisible
                    }
                }
        }


        /**************************************************************
         SCROLL BAR
         **************************************************************/
        ScrollBar {
            id:scrollbar
            flickable: list;
            visible: list.flicking
            opacity: 1
            scale: .98
            width: 5
            y: 25;
        }


        /**************************************************************
         MOUSE AREA
         **************************************************************/
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            preventStealing : true
            drag.target: parent
            propagateComposedEvents: false;
            z:1;
            enabled: list.interactive;

            property bool isPressed: false;

            onPressed: {
                if (dataModel.count) {
                    var obj = parent.mapToItem(list.contentItem, mouseX, mouseY)
                    activeIndex = list.indexAt(obj.x, obj.y);

                    var i=0;
                    var c=0;

                    if (activeIndex >= 0) {

                        if (groupSelectionEnabled){
                            if (groupShiftActive){
                                if (activeIndex > list.currentIndex){
                                    var last = -1;
                                    for(i=0; i<dataModel.count; i++){
                                        if (dataModel.get(i).held)
                                            last = i;
                                    }
                                    for(i=last; i<activeIndex; i++){
                                        dataModel.get(i).held = true;
                                    }
                                }
                                else if (activeIndex < list.currentIndex){
                                    var first = -1;
                                    for(i=0; i<dataModel.count; i++){
                                        if (dataModel.get(i).held){
                                            first = i;
                                            break;
                                        }
                                    }
                                    for(i=activeIndex+1; i<first; i++){
                                        dataModel.get(i).held = true;
                                    }
                                }
                            }

                            if (groupSelectionActive || groupShiftActive){
                                var hval = true;
                                hval = !dataModel.get(activeIndex).held;
                                dataModel.setProperty(activeIndex, "held", hval);
                            }
                            else{
                                if (dataModel.get(activeIndex) && dataModel.get(activeIndex).held){
                                }else{
                                    c=0;
                                    for(i=0; i<dataModel.count; i++){
                                        if (dataModel.get(i).held)
                                            c++;
                                    }
                                    for(i=0; i<dataModel.count; i++){
                                        dataModel.setProperty(i, "held", false);
                                    }
                                }
                                dataModel.setProperty(activeIndex, "held", true)
                            }
                        }
                        else {
                            dataModel.setProperty(activeIndex, "held", true)
                        }
                        list.currentIndex = activeIndex;

                        root.mousePressed();
                        isPressed = true;
                     }
                }
            }

            onPressAndHold: {
                var tPos = mapToItem(list.currentItem.tTex, mouseX, mouseY)
                var aPos = mapToItem(list.currentItem.aTex, mouseX, mouseY)
                var dPos = mapToItem(list.currentItem.dTex, mouseX, mouseY)
                var epass = false

                if (dPos.x >= 0) {
                    if (durationEditable && getCurrentMime().substring(0,5) == "image"){
                        list.currentItem.dTex.cursorVisible = true;
                        list.currentItem.dTex.forceActiveFocus();
                        epass = true;
                    }
                }
                else if (aPos.x >= 0)  {
                    list.currentItem.aTex.cursorVisible = true;
                    list.currentItem.aTex.forceActiveFocus();
                    epass = true;
                }
                else {
                    list.currentItem.tTex.cursorVisible = true;
                    list.currentItem.tTex.forceActiveFocus();
                    epass = true;
                }

                if (epass){
                    z = -1;
                    list.interactive = false;
                    root.editBegan();
                }
            }

            onReleased: {
                var i=0;
                var c=0;
                var moveGroup = [];

                if (list.currentIndex >= 0) {
                    root.activeIndex = activeIndex;
                    isPressed = false;
                    root.mouseReleased();
                }


                if (activeIndex == -2){ // end on lists
                    activeIndex = list.count-1;
                    for(i=0; i<dataModel.count; i++){
                        if (dataModel.get(i).held)
                            moveGroup.push(i);
                    }
                    for(i=moveGroup.length-1; i>=0; i--){
                       dataModel.move(moveGroup[i], activeIndex+c, 1);
                       c--;
                    }
                }
                else if (reorderIndicator.visible) {
                    if(activeIndex > list.currentIndex) {
                        for(i=0; i<dataModel.count; i++){
                            if (dataModel.get(i).held)
                                moveGroup.push(i);
                        }
                        for(i=moveGroup.length-1; i>=0; i--){
                           dataModel.move(moveGroup[i], activeIndex+c-1, 1);
                           c--;
                        }
                    }
                    else if (activeIndex < list.currentIndex) {
                        for(i=0; i<dataModel.count; i++){
                            if (dataModel.get(i).held)
                                moveGroup.push(i);
                        }
                        for(i=0; i<moveGroup.length; i++){
                           dataModel.move(moveGroup[i], activeIndex+c, 1);
                           c++;
                        }
                    }
                    reorderIndicator.visible = false;
                }



                if(!groupSelectionEnabled){
                    if (dataModel.get(list.currentIndex))
                      dataModel.get(list.currentIndex).held = false;
                }
                else if(groupSelectionEnabled && !groupSelectionActive && !groupShiftActive){
                    c=0;
                    for(i=0; i<dataModel.count; i++){
                        if (dataModel.get(i).held)
                            c++;
                    }
                    if (c > 1 && !reorder){
                        for(i=0; i<dataModel.count; i++){
                            dataModel.setProperty(i, "held", false);
                        }
                    }
                }

                dragIcon.visible = false;
            }

            onPositionChanged: {
                if (list.currentIndex >= 0) {
                    if(reorder && isPressed) {
                        var mouseItem = parent.mapToItem(list.contentItem, mouseX, mouseY)
                        var mouseHoverIndex = list.indexAt(0, mouseItem.y)
                        var mouseHoverItem = list.itemAt(0, mouseItem.y);

                        var inc = mouseY;
                        if (mouseX < list.width){

                            if (inc >=0 && inc <= list.height) {
                                if ((mouseItem.y / 24) - parseInt(mouseItem.y / 24) < .5) {
                                    if (reorderIndicator != null)
                                        reorderIndicator.y = mouseHoverItem.y;
                                }
                                else{
                                    if (reorderIndicator != null)
                                        reorderIndicator.y = mouseHoverItem.y+24;
                                }
                                if (reorderIndicator != null){
                                    activeIndex = list.indexAt(0, reorderIndicator.y);
                                    reorderIndicator.y = reorderIndicator.y - list.contentY;
                                }
                            }

                            if (mouseY+list.contentY >= list.contentHeight){
                               activeIndex = -2;
                            }
                            if (activeIndex == -2){
                                reorderIndicator.visible = true;
                            }
                            else if (dataModel.get(activeIndex) && !dataModel.get(activeIndex).held)
                                reorderIndicator.visible = true;
                            else
                                reorderIndicator.visible = false;
                        }
                        else {
                            reorderIndicator.visible = false;
                        }
                     }

                }
                root.mousePositionChanged();
                if (dragIcon.visible) {
                   dragIcon.x = mouseX;
                   dragIcon.y = mouseY;
                }
            }

            onDoubleClicked: {
                root.mouseDoubleClicked();
            }

        }
    }

    /**************************************************************
     GET CURRENT INDEX
     **************************************************************/
    function getCurrentIndex(){
        return list.currentIndex;
    }

    /**************************************************************
     HAS NEXT
     **************************************************************/
    function hasNext(){
        if (getCurrentIndex() < getCount()-1)
            return true;
        return false;
    }

    /**************************************************************
     HAS PREVIOUS
     **************************************************************/
    function hasPrevious(){
        if (getCurrentIndex() > 0)
            return true;
        return false;
    }

    /**************************************************************
     PREVIOUS INDEX
     **************************************************************/
    function prevIndex(){
        list.decrementCurrentIndex();
        if (groupSelectionEnabled){
            clearHeld();
            dataModel.get(list.currentIndex).held = true;
        }
    }

    /**************************************************************
     NEXT INDEX
     **************************************************************/
    function nextIndex(){
        list.incrementCurrentIndex();
        if (groupSelectionEnabled){
            clearHeld();
            dataModel.get(list.currentIndex).held = true;
        }
    }

    /**************************************************************
     CLEAR LIST
     **************************************************************/
    function clear(){
        list.currentIndex = 0;
        list.myData.clear();
    }

    /**************************************************************
     REMOVE CURRENT INDEX
     **************************************************************/
    function removeCurrentIndex(){
        if (list.currentIndex < 0 || list.currentIndex >= list.count)
            return;
        list.myData.remove(list.currentIndex);
        if (list.currentIndex != 0)
            list.decrementCurrentIndex();
    }

    /**************************************************************
     GET CURRENT TITLE
     **************************************************************/
    function getCurrentTitle() {
        if (dataModel.get(list.currentIndex))
            return dataModel.get(list.currentIndex).title;
        else
            return "";
    }

    /**************************************************************
     GET CURRENT AUTHOR
     **************************************************************/
    function getCurrentAuthor() {
        return dataModel.get(list.currentIndex).author;
    }

    /**************************************************************
     GET CURRENT DURATION
     **************************************************************/
    function getCurrentDuration() {
        return dataModel.get(list.currentIndex).duration;
    }

    /**************************************************************
     GET CURRENT FILE
     **************************************************************/
    function getCurrentFile() {
        return dataModel.get(list.currentIndex).file;
    }

    /**************************************************************
     GET CURRENT MIME
     **************************************************************/
    function getCurrentMime() {
        return dataModel.get(list.currentIndex).mime;
    }

    /**************************************************************
     GET CURRENT ID
     **************************************************************/
    function getCurrentId() {
        return dataModel.get(list.currentIndex).id;
    }

    /**************************************************************
     GET CURRENT HELD FLAG
     **************************************************************/
    function getCurrentHeld() {
        return dataModel.get(list.currentIndex).held;
    }

    /**************************************************************
     GET FILE BY INDEX
     **************************************************************/
    function getFileByIndex(index) {
        return dataModel.get(index).file;
    }

    /**************************************************************
     SET LIST PROPERTY
     **************************************************************/
    function setProperty(index, prop, value) {
        dataModel.setProperty(index, prop, value);
    }

    /**************************************************************
     CLEAR HELD PROPERTY ON ALL
     **************************************************************/
    function clearHeld() {
        for(var i=0; i<dataModel.count; i++){
            dataModel.setProperty(i, "held", false);
        }
    }

    /**************************************************************
     SET CURRENT INDEX
     **************************************************************/
    function setCurrentIndex(i) {
        list.currentIndex = i;
        if (groupSelectionEnabled){
            clearHeld();
            if (dataModel.get(i))
               dataModel.get(i).held = true;
        }
    }
}
