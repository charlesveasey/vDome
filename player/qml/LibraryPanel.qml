import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.1

/**************************************************************
 LIBRARY PANEL
 **************************************************************/
Item {
    property alias active: libraryTab.checked;
    property alias groupSelectionActive: libraryList.groupSelectionActive;
    property alias groupShiftActive: libraryList.groupShiftActive;

    property real slide: 10;
    property real cSort: 0;

    signal editBegan;
    signal editEnded;
    signal mousePressed;
    signal mouseReleased;
    signal mouseDoubleClicked;

    id: root;
    x: divider.x+5;
    y:105;
    width: parent.width - x - 5;
    height: parent.height - 195;
    visible: width > 10;
    opacity: width <= 200 ? width / 200 : 1

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
     BACKGROUND
     **************************************************************/
    Rectangle {
        color:"black"
        x: 10;
        width:parent.width
        height:parent.height + 195
    }

    /**************************************************************
     LIBRARY TAB
     **************************************************************/
    TabButton {
        id: libraryTab;
        x: 19;
        y: checked ? 15 : 17;
        text: qsTr("Media")
        hit.width: 65; hit.height: 50; hit.x: -5; hit.y: -25;
        checked: true;
        onClicked: {
            playlistIndexPanel.active = false;
        }
    }

    /**************************************************************
     LIBRARY LIST
     **************************************************************/
    ListPanel {
        id: libraryList;
        x:0;y:58;
        width:parent.width;
        height:parent.height;
        visible: active;
        allowDrag: true;
        reorder: false;
        groupSelectionEnabled: true;

        onEditBegan: {
            root.editBegan();
        }
        onEditEnded: {
            root.editEnded();
            updateCurrentItem();
        }
        onMousePressed: {
            root.mousePressed();
        }
        onMouseReleased: {
            root.mouseReleased();
            if (mouseDraggedToPlaylist()){
                var l = libraryList.list;
                for (var i=0; i<l.count; i++){
                    if (l.model.get(i).held) {
                        playlistPanel.list.myData.append({
                                "index":    playlistPanel.list.count+1,
                                "title":    l.model.get(i).title,
                                "author":   l.model.get(i).author,
                                "duration": l.model.get(i).duration,
                                "file":     l.model.get(i).file,
                                "mime":     l.model.get(i).mime,
                                "held":     false
                        })
                    }
                }
                playlistPanel.sumPlaylistDuration();
            }
        }
        onMouseDoubleClicked: {
            controlPanel.currentPanel = libraryPanel;
            controlPanel.title = getCurrentTitle();
            controlPanel.playFile();
        }
        onMousePositionChanged: {
            mouseDraggedToPlaylist();
        }

        onCompleted: {
            db.createLibrary();
            loadLibraryByTitle(false);
        }
        onFileDragged: {
            importFiles(files);
        }
        onTitleClicked: {
            if (cSort != 0){
                loadLibraryByTitle(false);
                cSort = 0;
            }
            else{
                loadLibraryByTitle(true);
                cSort = 1;
            }
        }
        onAuthorClicked: {
            if (cSort != 2){
                loadLibraryByAuthor(false);
                cSort = 2;
            }
            else{
                loadLibraryByAuthor(true);
                cSort = 3;
            }
        }
        onDurationClicked: {
            if (cSort != 4){
                loadLibraryByDuration(false);
                cSort = 4;
            }
            else{
                loadLibraryByDuration(true);
                cSort = 5;
            }
        }
    }

    /**************************************************************
     ADD / REMOVE BUTTONS
     **************************************************************/
    Item {
        id: addRemoveBtns;
        x: -3
        y: parent.height + 58
        visible: active && window.height > 275

        ImageButton { x: 25; y: 0; imageSource: "../images/item-add.png"
            hit.width:width+6; hit.height: height+20; hit.x: -3; hit.y: -8;
            onClicked: {
                add();
            }
        }
        ImageButton { x: 58; y: 6; imageSource: "../images/item-remove.png"
            hit.width:width+6; hit.height: height+20; hit.x: -3; hit.y: -8;
            onClicked: {
                remove();
            }
        }
    }

    /**************************************************************
     REMOVE WARNING DIALOG
     **************************************************************/
    MessageDialog {
        id: removeWarningDialog
        title: "Remove Item From Library"
        //standardButtons: StandardButton.Yes | StandardButton.No

        onVisibleChanged: {
            standardButtons = StandardButton.Yes | StandardButton.No
        }

        onYes: {
            removeCurrentLibraryItem();
            /*
            deleteFileDialog.text = "Do you want to delete '"+ getCurrentFile() +
               "' from the hard drive?\nWARNING: The file cannot be recovered!"
            deleteFileDialog.open();
            */
        }
        Component.onCompleted: visible = false;
    }

    /**************************************************************
     DELETE WARNING DIALOG
     **************************************************************/
    MessageDialog {
        id: deleteFileDialog
        title: "Delete File From Hard Drive"
        onVisibleChanged: {
            standardButtons = StandardButton.Yes | StandardButton.No
        }
        onYes: {
            syscmds.deleteFile(getCurrentFile());
            removeCurrentLibraryItem();
        }
        onNo: {
            removeCurrentLibraryItem();
        }

        Component.onCompleted: visible = false;
    }

    /**************************************************************
     FILE DIALOG
     **************************************************************/
    FileDialog {
        id: fileDialogLibrary
        title: "Please choose a file"
        selectMultiple: true;
        selectFolder: false;
        onAccepted: {
            importFiles(fileUrls);
        }
    }

    /**************************************************************
     DROP AREA
     **************************************************************/
    DropArea {
       id: dropArea;
       width:parent.width
       height:parent.height
       onEntered: {
           drag.accept (Qt.CopyAction);
       }
       onDropped: {
           drop.accept(Qt.CopyAction);
           importFiles(drop.urls);
       }
       onExited: {
           Drag.drop()
       }
    }

    /**************************************************************
     IMPORT FILES
     **************************************************************/
    function importFiles(fileUrls){
        var filepath = "";
        var filename = "";
        var mtype = "";
        var dur = 0;

        for (var i=0; i<fileUrls.length; i++){
            filepath = Qt.resolvedUrl(fileUrls[i]);
            if (!db.searchLibraryByFile(filepath)){
                filename = formatURL(filepath);
                mtype = plist.getMimeType(filename);
                if(mtype.substring(0,5) == "image"){
                    dur = slide;
                }
                libraryList.model.append({
                           "title": filename,
                           "author": "",
                           "duration": dur,
                           "file":  plist.convertPath(filepath),
                           "mime": mtype,
                           "held": false
                });
                db.insertLibraryItem(filename, "", dur, plist.convertPath(filepath), mtype);
             }
            else{
                //console.log("file already in library", filepath);
            }
        }
        loadSort();
    }

    /**************************************************************
     LOAD LIBRARY BY TITLE
     **************************************************************/
    function loadLibraryByTitle(desc){
        loadLibrary(db.selectLibraryOrderByTitle(desc));
    }

    /**************************************************************
     LOAD LIBRARY BY AUTHOR
     **************************************************************/
    function loadLibraryByAuthor(desc){
        loadLibrary(db.selectLibraryOrderByAuthor(desc));
    }

    /**************************************************************
     LOAD LIBRARY BY DURATION
     **************************************************************/
    function loadLibraryByDuration(desc){
        loadLibrary(db.selectLibraryOrderByDuration(desc));
    }

    /**************************************************************
     LOAD LIBRARY
     **************************************************************/
    function loadLibrary(rows){
        libraryList.model.clear();
        var title = "";
        var author = "";
        for(var i = 0; i < rows.length; i++) {
            title = rows.item(i).title === null ? "" : rows.item(i).title;
            author = rows.item(i).author === null ? "" : rows.item(i).author;
            libraryList.model.append({
                 "title":       title,
                 "author":      author,
                 "duration":    rows.item(i).duration,
                 "file":        rows.item(i).file,
                 "mime":        rows.item(i).mime,
                 "held":        false
             });
        }
    }

    /**************************************************************
     REMOVE LIBRARY ITEM
     **************************************************************/
    function removeCurrentLibraryItem(){
        db.deleteLibraryItem(libraryList.getCurrentFile());
        libraryList.removeCurrentIndex();
    }


    /**************************************************************
     IMPORT LIBRARY
     **************************************************************/
    function importLibrary(filepath){
        clear();
        var arr = plist.parse(filepath);
        for(var i=0; i<arr.length; i+=5){
                libraryList.model.append(
                        {"duration":    parseFloat(arr[i]),
                         "author":      arr[i+1],
                         "title":       arr[i+2],
                         "file":        arr[i+3],
                         "mime":        arr[i+4]  });
        }
    }

    /**************************************************************
     EXPORT LIBRARY
     **************************************************************/
    function exportLibrary(filepath){
        var arr = [];
        var index = 0;
        for(var i = 0; i < libraryList.model.count; i++){
            arr[index] = libraryList.model.get(i).duration;
            arr[index+1] = libraryList.model.get(i).author;
            arr[index+2] = libraryList.model.get(i).title;
            arr[index+3] = libraryList.model.get(i).file;
            index = index+4;
        }
        plist.save(filepath, arr);
    }

    /**************************************************************
     UDPATE CURRENT ITEM
     **************************************************************/
    function updateCurrentItem(){
        db.updateLibraryItem(getCurrentTitle(),
                             getCurrentAuthor(),
                             getCurrentDuration(),
                             getCurrentFile(),
                             getCurrentMime());
    }

    /**************************************************************
     UDPATE DURATION
     **************************************************************/
    function updateDuration(){
        var mtype="";
        for (var i = 0; i < libraryList.model.count; i++){
            mtype = libraryList.model.get(i).mime;
            if(mtype.substring(0,5) == "image"){
                libraryList.model.get(i).duration = slide;
                db.updateLibraryItemDuration(libraryList.model.get(i).file, slide);
            }
        }
    }

    /**************************************************************
     UDPATE DURATION TEMP, DOESN'T WRITE TO SLIDE OR DB
     **************************************************************/
    function updateDurationTmp(value){
        var mtype="";
        for (var i = 0; i < libraryList.model.count; i++){
            mtype = libraryList.model.get(i).mime;
            if(mtype.substring(0,5) == "image"){
                libraryList.model.get(i).duration = value;
            }
        }
    }

    /**************************************************************
     GET CURRENT TITLE
     **************************************************************/
    function getCurrentTitle(){
        return libraryList.getCurrentTitle();
    }

    /**************************************************************
     GET CURRENT AUTHOR
     **************************************************************/
    function getCurrentAuthor() {
        return libraryList.getCurrentAuthor();
    }

    /**************************************************************
     GET CURRENT DURATION
     **************************************************************/
    function getCurrentDuration() {
        return libraryList.getCurrentDuration();
    }

    /**************************************************************
     GET CURRENT FILE
     **************************************************************/
    function getCurrentFile(){
        return libraryList.getCurrentFile();
    }

    /**************************************************************
     GET CURRENT MIME
     **************************************************************/
    function getCurrentMime(){
        return libraryList.getCurrentMime();
    }

    /**************************************************************
     SET INTERACTIVE
     **************************************************************/
    function setInteractive(value){
        libraryList.list.interactive = value;
    }

    /**************************************************************
     SET CURRENT INDEX
     **************************************************************/
    function setCurrentIndex(value){
        libraryList.list.currentIndex = value;
    }

    /**************************************************************
     MOUSE DRAGGED TO PLAYLIST
     **************************************************************/
    function mouseDraggedToPlaylist(){
        if (libraryList.list.mouseArea.mouseX < -20 && libraryList.list.mouseArea.mouseY > -60) {
            libraryList.dragIconVisible = true;
            return true;
        }
        else {
            libraryList.dragIconVisible = false;
            return false;
        }
    }

    /**************************************************************
     GET LIST COUNT
     **************************************************************/
    function getCount(value){
        return libraryList.list.count
    }

    /**************************************************************
     GET FILE BY INDEX
     **************************************************************/
    function getFileByIndex(index) {
        return libraryList.getFileByIndex(index);
    }

    /**************************************************************
     SET LIST PROPERTY
     **************************************************************/
    function setProperty(index, prop, value) {
        libraryList.setProperty(index, prop, value)
    }

    /**************************************************************
     GET CURRENT INDEX
     **************************************************************/
    function getCurrentIndex(){
        return libraryList.getCurrentIndex();
    }

    /**************************************************************
     HAS NEXT
     **************************************************************/
    function hasNext(){
        return libraryList.hasNext();
    }

    /**************************************************************
     HAS PREVIOUS
     **************************************************************/
    function hasPrevious(){
        return libraryList.hasPrevious();
    }

    /**************************************************************
     PREVIOUS INDEX
     **************************************************************/
    function previousIndex() {
        return libraryList.prevIndex();
    }

    /**************************************************************
     NEXT INDEX
     **************************************************************/
    function nextIndex() {
        libraryList.nextIndex();
    }

    /**************************************************************
     RETURNS TITLE FORMATTED FOR CONTROL PANEL
     **************************************************************/
    function getControlPanelTitle() {
        return getCurrentTitle();
    }

    /**************************************************************
     PARSE AND LOAD CURRENT SORT
     **************************************************************/
    function loadSort(){
        var curIndex = getCurrentIndex();
        switch (cSort){
            case 0: loadLibraryByTitle(false);      break;
            case 1: loadLibraryByTitle(true);       break;
            case 2: loadLibraryByAuthor(false);     break;
            case 3: loadLibraryByAuthor(true);      break;
            case 4: loadLibraryByDuration(false);   break;
            case 5: loadLibraryByDuration(true);    break;
        }
        setCurrentIndex(curIndex);
    }

    /**************************************************************
     CLEAR HELD PROPERTY ON ALL
     **************************************************************/
    function clearHeld() {
        libraryList.clearHeld();
    }

    /**************************************************************
     CLEAR
     **************************************************************/
    function clear() {
        libraryList.clear();
    }

    /**************************************************************
     ADD
     **************************************************************/
    function add() {
        fileDialogLibrary.open();
    }

    /**************************************************************
     REMOVE
     **************************************************************/
    function remove() {
        if (getCount() && getCurrentIndex() >= 0){
            removeWarningDialog.text = "Do you want to remove '"+ getCurrentTitle() +
               "'\nfrom the library?"
            removeWarningDialog.open();
        }
    }
}
