import QtQuick 2.2

/**************************************************************
 CONTROL PANEL
 **************************************************************/
Item {
    property alias title: titleText.text;
    property alias time: timeText.text;
    property alias positionValue: pSlider.value;
    property alias volumeValue: vSlider.value;
    property alias sourceValue: inputItems.selectedItem;
    property alias playbackValue: playbackItems.selectedItem;
    property alias timerCnt: timer.timerCnt;
    property QtObject currentPanel: libraryPanel;
    property real cDuration: 0;
    property string cType;
    property int cIndex:-1;
    property bool isPlaying: false;
    property bool fileQueued;

    signal sourcePressed
    signal selectionPopup

    id: root
    width: parent.width;
    height: parent.height;
    focus: true;

    /**************************************************************
     KEYBOARD
     **************************************************************/
    Keys.onSpacePressed:  {
        if(playBtn.visible){
            pauseBtn.checked = true;
            glowTimer.start();
            playBtn.clicked();
         }
        else {
            playBtn.checked = true;
            glowTimer.start();
            pauseBtn.clicked();
        }
    }
    Keys.onLeftPressed:  {
        previousBtn.checked = true;
        glowTimer.start();
        previousBtn.clicked();
    }
    Keys.onRightPressed:  {
        nextBtn.checked = true;
        glowTimer.start();
        nextBtn.clicked();
    }
    Keys.onPressed: {
        if (event.modifiers == Qt.ControlModifier){
            libraryPanel.groupSelectionActive = true;
            playlistPanel.groupSelectionActive = true;
        }
        else if (event.modifiers == Qt.ShiftModifier){
            libraryPanel.groupShiftActive = true;
            playlistPanel.groupShiftActive = true;
        }
    }
    Keys.onReleased: {
        if (event.key == 16777249 || event.modifiers == Qt.ControlModifier){
            libraryPanel.groupSelectionActive = false;
            playlistPanel.groupSelectionActive = false;
        }
        else if (event.key == 16777248 || event.modifiers == Qt.ShiftModifier){
            libraryPanel.groupShiftActive = false;
            playlistPanel.groupShiftActive = false;
        }
    }

    /**************************************************************
     GLOW TIMER
     **************************************************************/
     Timer {
        property real timerCnt: 0;
        id: glowTimer
        interval: 250; running: false; repeat: false
        onTriggered: {
            previousBtn.checked = false;
            playBtn.checked = false;
            pauseBtn.checked = false;
            nextBtn.checked = false;
        }
     }

    /**************************************************************
     TIMER
     **************************************************************/
     Timer {
        property real timerCnt: 0;
        id: timer
        interval: 10; running: false; repeat: true
        onTriggered: {
            if (cType == "image") {
                timerCnt++
                positionValue = timerCnt/100/cDuration;
                time = secondsToHms(cDuration*positionValue);
                if (timerCnt/100 >= cDuration){
                    if (playbackValue == "loop file"){
                        resetTimer();
                    }
                    else{
                        end();
                    }
                }
            }
        }
     }

    /**************************************************************
     BACKGROUND
     **************************************************************/
    Image { source: "../images/top-bg.png"; width: parent.width; opacity: .6 }

    /**************************************************************
     PREVIOUS
     **************************************************************/
    ImageButton { id:previousBtn; x: 27; y: 39; imageSource: "../images/left-arrow.png"
        hit.width:width+8; hit.height: height+35; hit.x: -4; hit.y: -17;
        onClicked: {
            previousBtnHandler();
        }
    }

    /**************************************************************
     PLAY
     **************************************************************/
    ImageButton { id:playBtn; x: 74; y: 32; imageSource: "../images/play.png"
        hit.width:width+20; hit.height: height+20; hit.x: -12; hit.y: -10;
        visible: !isPlaying;
        onClicked: {
            play();
        }
    }

    /**************************************************************
     PAUSE
     **************************************************************/
    ImageButton { id:pauseBtn; x: 73; y: 33; imageSource: "../images/pause.png";
        hit.width:width+21; hit.height: height+23; hit.x: -12; hit.y: -12;
        visible: isPlaying;
        onClicked: {
            pause();
        }
    }

    /**************************************************************
     NEXT
     **************************************************************/
    ImageButton { id:nextBtn; x: 119; y: 36; imageSource: "../images/right-arrow.png"
        hit.width:width+8; hit.height: height+32; hit.x: -4; hit.y: -14;
        onClicked: {
            nextBtnHandler();
        }
    }

    /**************************************************************
     TITLE
     **************************************************************/
    Text { id: titleText; x: 193; y: 25;
        color: "#ffffff"
        text: qsTr("")
        font.pixelSize: 18
        font.family: openSansExtraBold.name;
        font.bold: true;
        width: pSlider.width-60;
        elide: Text.ElideRight;
    }

    /**************************************************************
     POSITION SLIDER
     **************************************************************/
    SliderCustom {
        id: pSlider
        x: 191; y: 54;
        width: vGroup.x-x-40; height: 30;
        minimum: 0.0; maximum: 1.0
        hit.height: height+8; hit.y: -12

        onPressedChanged: {
            if (pressed)
                socket.sendPause();
            else if (pauseBtn.visible)
                socket.sendPlay();
        }

        onValueChangedByHandle: {
            seek();
        }
    }

    Text { id: timeText; x: pSlider.x + pSlider.width-48; y: 34;
        color: "#ffffff"
        text: qsTr("00:00:00")
        font.pixelSize: 12
        font.family: openSansItalic.name
        font.italic: true;
        visible: pSlider.enabled
    }

    /**************************************************************
     VOLUME SLIDER
     **************************************************************/
    Item {
        id: vGroup;
        x: parent.width - vSlider.width-300
        y: 25;

        Text { id: volume;
            x: 3;
            y: 0;
            color: "#ffffff"
            opacity: vSlider.enabled ? 1.0 : 0.5
            text: qsTr("Volume")
            font.pixelSize: 18
            font.family: openSansExtraBold.name;
            font.bold: true;
        }

        SliderCustom {
            id: vSlider; y: 28
            width: 80; height: 30;
            minimum: 0.0; maximum: 1.0
            hit.height: height+8; hit.y: -12
            value: 1;

            onValueChangedByHandle: {
                socket.sendVolume(volumeValue)
            }
        }
    }

    /**************************************************************
     ADDITIONAL SETTINGS
     **************************************************************/
    Item {
        x: parent.width-395;


        /**************************************************************
         INPUT SOURCE
         **************************************************************/
        Text { id: input; x: 130; y: 25;
            color: "#ffffff"
            text: qsTr("Input")
            font.pixelSize: 18
            font.family: openSansExtraBold.name;
            font.bold: true;
        }

        TextButton {
            id: inputItem; x: input.x; y: 50;
            text: qsTr("media")
            hit.width: 65; hit.height: 50; hit.x: -5; hit.y: -25;
            onClicked: {
                inputItem.visible = false;
                inputItems.visible = true;
            }
        }

        SelectionPanel {
            id: inputItems; x: 104.5; y: 33;
            visible: false
            width: 100
            height: 168
            itemHeight: 40

            Component.onCompleted: {
                model.append({ name: 'media'   });
                model.append({ name: 'capture' });

                if (syscmds.getOSName() == 'osx')
                    model.append({ name: 'syphon'  });
                else if (syscmds.getOSName() == 'win')
                    model.append({ name: 'spout'  });

                model.append({ name: 'grid'    });
                model.append({ name: 'black'    });
                model.append({ name: 'white'    });
            }

            onSelectionPopup: {
                playbackItems.visible = false;
                playbackItem.visible = true;
                formatItems.visible = false;
                formatItem.visible = true;
                root.selectionPopup();
            }

            onClicked: {
                inputItem.text = selectedItem;
                inputItem.visible = true;
                inputItems.visible = false;

                if (selectedItem == "media"){
                    pSlider.enabled = true;
                    vSlider.enabled = true;
                    playBtn.enabled = true;
                    pauseBtn.enabled = true;
                    previousBtn.enabled = true;
                    nextBtn.enabled = true;
                    playbackItem.enabled = true;
                    title = "";
                    if (currentPanel && currentPanel.getControlPanelTitle())
                        title = currentPanel.getControlPanelTitle();
                }
                else{
                    pSlider.enabled = false;
                    vSlider.enabled = false;
                    playBtn.enabled = false;
                    pauseBtn.enabled = false;
                    previousBtn.enabled = false;
                    nextBtn.enabled = false;
                    playbackItem.enabled = false;
                    title = selectedItem.toUpperCase();
                }

                if (selectedItem == "grid" || selectedItem == "black" || selectedItem == "white"){
                    formatItem.text = "domemaster";
                    formatItem.enabled = false;
                    formatItems.selectedItem = "domemaster";
                    formatItems.list.currentIndex = 0;
                }
                else{
                    formatItem.enabled = true;
                }

                socket.sendSource(sourceValue);
                root.sourcePressed();
            }

            onExited: {
                inputItem.visible = true;
            }
        }


        /**************************************************************
         INPUT FORMAT
         **************************************************************/
        Text { id: formatHeader; x: 212; y: 25;
            color: "#ffffff"
            opacity: formatItem.enabled ? 1.0 : 0.5
            text: qsTr("Format")
            font.pixelSize: 18
            font.family: openSansExtraBold.name;
            font.bold: true;
        }

        TextButton {
            id: formatItem; x: formatHeader.x; y: 50;
            color: "#ffffff"
            opacity: enabled ? 1.0 : 0.5
            text: qsTr("domemaster")
            hit.width: 90; hit.height: 50; hit.x: -5; hit.y: -25;
            onClicked: {
                formatItem.visible = false;
                formatItems.visible = true;
            }
        }

        SelectionPanel {
            id: formatItems; x: formatHeader.x-32; y: 33.25;
            visible: false
            width: 125
            height: 75
            itemHeight: 40

            Component.onCompleted: {
                model.append({ name: 'domemaster'   });
                model.append({ name: 'HD' });
                //socket.sendFormat("domemaster");
                selectedItem = 'domemaster';
            }

            onSelectionPopup: {
                inputItems.visible = false;
                inputItem.visible = true;
                playbackItems.visible = false;
                playbackItem.visible = true;
                root.selectionPopup();
            }
            onClicked: {
                formatItem.text = selectedItem;
                formatItem.visible = true;
                formatItems.visible = false;
                if (selectedItem == "domemaster") {
                    socket.sendFormat("domemaster");
                }
                else if (selectedItem == "HD") {
                    socket.sendFormat("hd");
                }
            }

            onExited: {
                formatItem.visible = true;
            }
        }


        /**************************************************************
         MEDIA PLAYBACK TYPE
         **************************************************************/
        Text { id: playbackHeader; x: 315; y: 25;
            color: "#ffffff"
            opacity: playbackItem.enabled ? 1.0 : 0.5
            text: qsTr("Play")
            font.pixelSize: 18
            font.family: openSansExtraBold.name;
            font.bold: true;
        }

        TextButton {
            id: playbackItem; x: playbackHeader.x+1; y: 50;
            opacity: enabled ? 1.0 : 0.5
            text: qsTr("once")
            hit.width: 65; hit.height: 50; hit.x: -5; hit.y: -25;
            onClicked: {
                playbackItem.visible = false;
                playbackItems.visible = true;
            }
        }

        SelectionPanel {
            id: playbackItems; x: playbackHeader.x-32; y: 33.25;
            visible: false
            width: 100
            height: 120
            itemHeight: 40

            Component.onCompleted: {
                model.append({ name: 'once'   });
                model.append({ name: 'loop file' });
                model.append({ name: 'loop list'  });
                model.append({ name: 'random'    });
                socket.sendLoop("off");
                selectedItem = 'once';
            }

            onSelectionPopup: {
                inputItems.visible = false;
                inputItem.visible = true;
                formatItems.visible = false;
                formatItem.visible = true;
                root.selectionPopup();
            }
            onClicked: {
                playbackItem.text = selectedItem;
                playbackItem.visible = true;
                playbackItems.visible = false;
                if (playbackValue == "loop file") {
                    socket.sendLoop("on");
                }
                else {
                    socket.sendLoop("off");
                }
            }

            onExited: {
                playbackItem.visible = true;
            }
        }

    }

    /**************************************************************
     CLOSE SELECTION POPUP
     **************************************************************/
    function closeSelectionPopup(){
        inputItems.visible = false;
        inputItem.visible = true;
        playbackItems.visible = false;
        playbackItem.visible = true;
    }

    /**************************************************************
     START TIMER
     **************************************************************/
    function startTimer() {
        if (cType == "image")
           timer.start();
        isPlaying = true;
    }

    /**************************************************************
     STOP TIMER
     **************************************************************/
    function stopTimer() {
        timer.stop();
        isPlaying = false;
    }

    /**************************************************************
     RESET TIMER
     **************************************************************/
    function resetTimer() {
        timerCnt = 0;
        positionValue = 0;
        time = secondsToHms(0);
    }

    /**************************************************************
     PLAY FILE
     **************************************************************/
    function playFile() {
        fileQueued = false;

        if (currentPanel == playlistIndexPanel){
            currentPanel = playlistPanel;
            playlistPanel.setCurrentIndex(0);
        }

        cDuration = currentPanel.getCurrentDuration();
        cType = currentPanel.getCurrentMime();
        cType = cType.substring(0,5);
        cIndex = currentPanel.getCurrentIndex();

        title = currentPanel.getControlPanelTitle();
        stopTimer();
        resetTimer();
        startTimer();
        socket.sendFile(currentPanel.getCurrentFile());
    }

    /**************************************************************
     PLAY
     **************************************************************/
    function play() {
        startTimer();
        if (fileQueued){
            playFile()
            socket.sendFile(currentPanel.getCurrentFile());
        }
        else
            socket.sendPlay();
    }

    /**************************************************************
     PAUSE
     **************************************************************/
    function pause() {
        stopTimer();
        socket.sendPause();
    }

    /**************************************************************
     PREVIOUS BUTTON HANDLER
     **************************************************************/
    function previousBtnHandler() {
        if (!currentPanel) return;

        if(currentPanel == libraryPanel || currentPanel == playlistPanel){
            if (playbackValue == "once" || playbackValue == "loop file"){
                if (currentPanel.hasPrevious())
                    currentPanel.previousIndex();
            }
            else if (playbackValue == "loop list"){
                if (currentPanel.hasPrevious())
                    currentPanel.previousIndex();
                else
                    currentPanel.setCurrentIndex(currentPanel.getCount()-1)
            }
            else if (playbackValue == "random")
                nextRandom();
        }
        else{
            if (currentPanel.hasPrevious())
                currentPanel.previousIndex();
        }

        if (isPlaying)
           playFile();
        else
            fileQueued = true;

        title = currentPanel.getControlPanelTitle();
    }

    /**************************************************************
     NEXT BUTTON HANDLER
     **************************************************************/
    function nextBtnHandler() {
        if (!currentPanel) return;

        if(currentPanel == libraryPanel || currentPanel == playlistPanel){
            if (playbackValue == "once" || playbackValue == "loop file"){
                if (currentPanel.hasNext())
                    currentPanel.nextIndex();
            }
            else if (playbackValue == "loop list"){
                if (currentPanel.hasNext())
                    currentPanel.nextIndex();
                else
                    currentPanel.setCurrentIndex(0);
            }
            else if (playbackValue == "random")
                nextRandom();
        }
        else{
            if (currentPanel.hasNext())
                currentPanel.nextIndex();
        }

        if (isPlaying)
           playFile();
        else
            fileQueued = true;

        title = currentPanel.getControlPanelTitle();

    }

    /**************************************************************
     NEXT
     **************************************************************/
    function next() {
        if (!currentPanel) return;

        if (playbackValue == "once"){ // only playlist panel
            if (currentPanel.hasNext()) {
                currentPanel.nextIndex();
                playFile();
            }
            else{
                playlistPanel.setCurrentIndex(0);
                fileQueued = true;
                title = currentPanel.getControlPanelTitle();
            }
        }
        else if (playbackValue == "loop list"){
            if (currentPanel.hasNext())
                currentPanel.nextIndex();
            else
                currentPanel.setCurrentIndex(0);
            playFile();
        }
        else if (playbackValue == "random") {
            nextRandom();
            playFile();
        }

    }

    /**************************************************************
     NEXT RANDOM
     **************************************************************/
    function nextRandom() {
        var newIndex = -2;

        if (currentPanel.getCount() > 1){
            do{
                newIndex = Math.floor((Math.random() * currentPanel.getCount()) + 0);
            }while(newIndex == cIndex)
            cIndex = newIndex;
        }

        currentPanel.setCurrentIndex(cIndex)
    }

    /**************************************************************
     SEEK UPDATE FOR IMAGE TIMER
     **************************************************************/
    function seek() {
        timerCnt = (100*cDuration*positionValue).toFixed(2);
        socket.sendSeek(positionValue);
    }

    /**************************************************************
     END CALLBACK
     **************************************************************/
    function end() {
        if (!currentPanel) return;

        stopTimer();

        if(currentPanel == libraryPanel){
            if (playbackValue == "once"){
                isPlaying = false;
            }
            else if (playbackValue == "loop list" || playbackValue == "random"){
                next();
            }
        }
        else if(currentPanel == playlistPanel){
            if (playbackValue != "loop file")
                next();
        }
    }

}
