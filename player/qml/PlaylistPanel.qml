import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.1

/**************************************************************
 PLAYLIST PANEL
 **************************************************************/
Item {
    property alias title: playlistTitle.text;
    property alias list: playlist.list;
    property alias groupSelectionActive: playlist.groupSelectionActive;
    property alias groupShiftActive: playlist.groupShiftActive;

    property real cIndexId;
    property real cSum: 0;

    signal editBegan;
    signal editEnded;
    signal mousePressed;
    signal mouseDoubleClicked;

    id: root;
    width: divider.x - 15;
    height: parent.height;

    visible: width >= 10
    opacity: width <= 150 ? width / 150 : 1

    /**************************************************************
     HEADER
     **************************************************************/
    Text {
        x: 25; y: 120;
        color: "#ffffff"
        text: qsTr("Playlist")
        font.pixelSize: 18
        font.family: openSansExtraBold.name;
        font.bold: true;
    }

    /**************************************************************
     PLAYLIST TITLE
     **************************************************************/
    TextInput{
        id:playlistTitle
        x: 25; y: 150;
        color: "#ffffff";
        text: qsTr("My Playlist")
        font.pixelSize: 17
        font.family: openSansSemiBold.name;
        font.bold: true;
        z: 0
        activeFocusOnPress: false
        width: parent.width - 40;
        cursorVisible: false
        readOnly: false;
        clip: true;
        selectByMouse: true;
        visible: cursorVisible;
        onAccepted: {
            cursorVisible = false;
            playlistTitleMouseArea.enabled = true;
        }
    }
    Text {
        id: playlistTitleT
        x: playlistTitle.x
        y: playlistTitle.y
        width: playlistTitle.width
        color: playlistTitle.color
        text: playlistTitle.text
        font.pixelSize: playlistTitle.font.pixelSize
        font.family: playlistTitle.font.family
        font.bold: playlistTitle.font.bold
        z: playlistTitle.z
        elide: Text.ElideRight
        visible: !playlistTitle.cursorVisible;

        MouseArea {
            id: playlistTitleMouseArea;
            anchors.fill: parent
            z:1;
            onPressAndHold: {
                playlistTitle.forceActiveFocus();
                playlistTitle.cursorVisible = true;
            }
        }
    }

    /**************************************************************
     PLAYLIST DURATION
     **************************************************************/
    Text{
        id:playlistDuration
        x: playlist.durationHeaderX+playlist.x+2; y: 150;
        color: "#ffffff";
        text: secondsToHms(cSum);
        font.pixelSize: 15
        font.family: openSansSemiBold.name;
        font.bold: true;
        z: 0
        clip: true;
        visible: playlist.durationHeaderVisible;
    }

    /**************************************************************
     PLAYLIST LIST PANEL
     **************************************************************/
    ListPanel {
        id: playlist;
        x:5; y:200;
        width:parent.width; height:parent.height - 233;
        reorder: true;
        allowDrag: false;
        allowSort: false;
        durationEditable: true;
        groupSelectionEnabled: true;

        onEditBegan: {
            root.editBegan();
        }
        onEditEnded: {
            root.editEnded();
            sumPlaylistDuration();
        }
        onMousePressed: {
            root.mousePressed();
        }
        onMouseDoubleClicked: {
            controlPanel.currentPanel = playlistPanel;
            controlPanel.playFile();
        }
        onCompleted: {
            db.createPlaylist();
        }
    }

    /**************************************************************
     LIST MODIFICATION BUTTONS
     **************************************************************/
    Item {
        y: parent.height - 32;
        visible: root.width > 155 && root.height > 275
        //opacity: root.width <= 155 ? root.width / 155 + 155 : 1


        /**************************************************************
         ADD BUTTON
         **************************************************************/
        ImageButton { x: 25; y: 0; imageSource: "../images/item-add.png"
            hit.width:width+6; hit.height: height+20; hit.x: -3; hit.y: -8;
            onClicked: {
                add();
            }
        }

        /**************************************************************
         REMOVE BUTTON
         **************************************************************/
        ImageButton { x:58; y: 6; imageSource: "../images/item-remove.png"
            hit.width:width+6; hit.height: height+16; hit.x: -3; hit.y: -7;
            onClicked: {
                remove();
            }
        }

        /**************************************************************
         OPEN BUTTON
         *************************************************************
        ImageButton { x: 25; y: 0; imageSource: "../images/file-open.png";
            onClicked: {    
                fileDialog.open();
            }
        }*/

        /**************************************************************
         SAVE BUTTON
         **************************************************************/
        ImageButton { x:root.width-55; y: 1.5; imageSource: "../images/file-save.png"
            onClicked: {
                save();
            }
        }

        /**************************************************************
         CLOSE/CLEAR BUTTON
         **************************************************************/
        ImageButton { x:  root.width-25; y: 0; imageSource: "../images/file-close.png"
            onClicked: {
                clear();
            }
        }


    }

    /**************************************************************
     CLEAR WARNING DIALOG
     **************************************************************/
    MessageDialog {
        id: clearWarningDialog
        title: "Clear Playlist"
        text: "Do you want to clear the playlist?"
        onVisibleChanged: {
            standardButtons = StandardButton.Yes | StandardButton.No
        }
        onYes: {
            playlist.clear();
            cSum = 0;
        }
        Component.onCompleted: visible = false;
    }

    /**************************************************************
     REMOVE WARNING DIALOG
     **************************************************************/
    MessageDialog {
        id: removeWarningDialog
        title: "Remove Item From Playlist"
        onVisibleChanged: {
            standardButtons = StandardButton.Yes | StandardButton.No
        }
        onYes: {
            removeCurrentItem();
            sumPlaylistDuration();
        }
        Component.onCompleted: visible = false;
    }

    /**************************************************************
     ADD FILE DIALOG
     **************************************************************/
    FileDialog {
        id: fileAddDialog
        title: "Please Choose A File"
        selectMultiple: false;
        selectFolder: false;
        onAccepted: {
            importFiles(fileAddDialog.fileUrls);
            sumPlaylistDuration();
        }
    }

    /**************************************************************
     IMPORT PLAYLIST FILE DIALOG
     **************************************************************/
    FileDialog {
        id: fileImportDialog
        title: "Please Choose A File"
        selectMultiple: false;
        selectFolder: false;
        onAccepted: {
            importPlaylist(Qt.resolvedUrl(fileUrl));
            sumPlaylistDuration();
        }
    }

    /**************************************************************
     EXPORT FILE DIALOG
     **************************************************************/
    FileDialog {
        id: fileDialogExport
        title: "Please Choose A File"
        selectMultiple: false;
        selectFolder: false;
        selectExisting: false;
        onAccepted: {
            exportPlaylist(Qt.resolvedUrl(fileUrl));
        }
    }

    /**************************************************************
     DROP AREA
     **************************************************************/
    DropArea {
       id: dropArea;
       anchors.fill: parent;
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
     LOAD PLAYLIST
     **************************************************************/
    function loadPlaylist(indexId){
        var title = "";
        var author = "";
        var rows = db.selectPlaylistByIndexId(indexId);
        for(var i = 0; i < rows.length; i++) {
            title = rows.item(i).title === null ? "" : rows.item(i).title;
            author = rows.item(i).author === null ? "" : rows.item(i).author;
            list.model.append({
                 "id":          rows.item(i).id,
                 "title":       title,
                 "author":      author,
                 "duration":    parseFloat(rows.item(i).duration),
                 "file":        rows.item(i).file,
                 "mime":        rows.item(i).mime,
                 "indexId":     rows.item(i).indexId,
                 "held":        false
                });
        }
        setCurrentIndex(-1);
        sumPlaylistDuration();
    }

    /**************************************************************
     SAVE PLAYLIST
     **************************************************************/
    function savePlaylist(indexId){
        for(var i=0; i<playlist.model.count; i++){
            db.insertPlaylistItem(
                        indexId,
                        playlist.model.get(i).title,
                        playlist.model.get(i).author,
                        parseFloat(playlist.model.get(i).duration),
                        playlist.model.get(i).file,
                        playlist.model.get(i).mime,
                        i)
        }
    }

    /**************************************************************
     SUM PLAYLIST DURATION
     **************************************************************/
    function sumPlaylistDuration(){
        var sum = 0;
        for(var i=0; i<playlist.model.count; i++){
            sum +=  parseFloat(playlist.model.get(i).duration);
        }
        cSum = sum;
        return sum;
    }

    /**************************************************************
     IMPORT PLAYLIST
     **************************************************************/
    function importPlaylist(filepath){
        playlist.model.clear();
        var myArray = plist.parse(filepath);
        for(var i = 0; i < myArray.length; i+=5){
                playlist.model.append(
                        {"duration": myArray[i],
                         "author": myArray[i+1],
                         "title": myArray[i+2],
                         "file": myArray[i+3],
                         "mime": myArray[i+4],
                        });
        }
    }

    /**************************************************************
     EXPORT PLAYLIST
     **************************************************************/
    function exportPlaylist(filepath){
        var arr = [];
        var index = 0;
        for(var i = 0; i < playlist.model.count; i++){
            arr[index] =  parseFloat(playlist.model.get(i).duration);
            arr[index+1] = playlist.model.get(i).author;
            arr[index+2] = playlist.model.get(i).title;
            arr[index+3] = playlist.model.get(i).file;
            index = index+4;
        }
        plist.save(filepath +".m3u", arr);
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
                filename = formatURL(filepath);
                mtype = plist.getMimeType(filename);
                if(mtype.substring(0,5) == "image"){
                    dur = libraryPanel.slide;
                }
                playlist.model.append({
                           "title": filename,
                           "author": "",
                           "duration": dur,
                           "file":  plist.convertPath(filepath),
                           "mime": mtype,
                           "held": false
                });
        }
    }

    /**************************************************************
     REMOVE CURRENT ITEM
     **************************************************************/
    function removeCurrentItem(){
        playlist.removeCurrentIndex();
    }

    /**************************************************************
     GET CURRENT TITLE
     **************************************************************/
    function getCurrentTitle() {
        return playlist.getCurrentTitle();
    }

    /**************************************************************
     GET CURRENT AUTHOR
     **************************************************************/
    function getCurrentAuthor() {
        return playlist.getCurrentAuthor();
    }

    /**************************************************************
     GET CURRENT FILE
     **************************************************************/
    function getCurrentFile() {
        return playlist.getCurrentFile();
    }

    /**************************************************************
     GET CURRENT DURATION
     **************************************************************/
    function getCurrentDuration() {
        return playlist.getCurrentDuration();
    }

    /**************************************************************
     GET CURRENT MIME
     **************************************************************/
    function getCurrentMime() {
        return playlist.getCurrentMime();
    }

    /**************************************************************
     GET CURRENT ID
     **************************************************************/
    function getCurrentId() {
        return playlist.getCurrentId()
    }

    /**************************************************************
     GET CURRENT INDEX ID
     **************************************************************/
    function getCurrentIndexId() {
        return playlist.model.get(getCurrentIndex()).indexId;
    }

    /**************************************************************
     SET INTERACTIVE
     **************************************************************/
    function setInteractive(value){
        playlist.list.interactive = value;
    }

    /**************************************************************
     SET CURRENT INDEX
     **************************************************************/
    function setCurrentIndex(value){
        playlist.setCurrentIndex(value);
    }

    /**************************************************************
     GET LIST COUNT
     **************************************************************/
    function getCount(value){
        return playlist.list.count;
    }

    /**************************************************************
     GET FILE BY INDEX
     **************************************************************/
    function getFileByIndex(index) {
        return playlist.getFileByIndex(index);
    }

    /**************************************************************
     SET LIST PROPERTY
     **************************************************************/
    function setProperty(index, prop, value) {
        playlist.setProperty(index, prop, value);
    }

    /**************************************************************
     GET CURRENT INDEX
     **************************************************************/
    function getCurrentIndex(){
        return playlist.getCurrentIndex();
    }

    /**************************************************************
     HAS NEXT
     **************************************************************/
    function hasNext(){
        return playlist.hasNext();
    }

    /**************************************************************
     HAS PREVIOUS
     **************************************************************/
    function hasPrevious(){
        return playlist.hasPrevious();
    }

    /**************************************************************
     PREVIOUS INDEX
     **************************************************************/
    function previousIndex() {
        return playlist.prevIndex();
    }

    /**************************************************************
     NEXT INDEX
     **************************************************************/
    function nextIndex() {
        playlist.nextIndex();
    }

    /**************************************************************
     RETURNS TITLE FORMATTED FOR CONTROL PANEL
     **************************************************************/
    function getControlPanelTitle() {

        if (title.length)
            return controlPanel.title = title + ": " + getCurrentTitle();
        return controlPanel.title = getCurrentTitle();
    }

    /**************************************************************
     ADD MEDIA FILES
     **************************************************************/
    function add() {
        fileAddDialog.open();
    }

    /**************************************************************
     REMOVE PLAYLIST ITEM
     **************************************************************/
    function remove() {
        if (getCount() && getCurrentIndex() >= 0){
            removeWarningDialog.text = "Do you want to remove '"+ getCurrentTitle() +
               "'\nfrom the playlist?"
            removeWarningDialog.open();
        }
    }

    /**************************************************************
     SAVE PLAYLIST
     **************************************************************/
    function save() {
        playlistIndexPanel.savePlaylistIndex();
    }

    /**************************************************************
     CLEAR PLAYLIST
     **************************************************************/
    function clear() {
        if (getCount())
            clearWarningDialog.open();
    }

    /**************************************************************
     IMMPORT PLAYLIST
     **************************************************************/
    function importP() {
        fileImportDialog.open();
    }

    /**************************************************************
     EXPORT PLAYLIST
     **************************************************************/
    function exportP() {
        fileDialogExport.open();
    }

}
