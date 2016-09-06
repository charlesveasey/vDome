import QtQuick 2.2
import QtQuick.Dialogs 1.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.1

/**************************************************************
 PLAYLIST INDEX PANEL
 **************************************************************/
Item {
    property alias active: playlistTab.checked;
    property real cSort: 0;

    signal editBegan
    signal editEnded
    signal mousePressed
    signal mouseReleased
    signal mouseDoubleClicked

    id: root
    x: libraryPanel.x
    y: libraryPanel.y
    width: libraryPanel.width
    height: libraryPanel.height
    opacity: libraryPanel.opacity
    visible: width > 0

    /**************************************************************
     PLAYLIST INDEX TAB
     **************************************************************/
    TabButton {
        id: playlistTab;
        x: 95;
        y: checked ? 15 : 17;
        text: qsTr("Playlists")
        hit.width: 65; hit.height: 50; hit.x: -5; hit.y: -25;
        checked: false;
        onClicked: {
           libraryPanel.active = false;
        }
    }

    /**************************************************************
     PLAYLIST INDEX LIST
     **************************************************************/
    ListPanel {
        id: playlistIndex;
        x:0; y:58;
        width: parent.width;
        height: parent.height;
        visible: active;
        allowDrag: true
        reorder: false;
        titleHeaderText: "Name"

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
        onMouseDoubleClicked: {
            root.mouseDoubleClicked();
            controlPanel.currentPanel = playlistIndexPanel;
            playlistPanel.list.model.clear();
            playlistPanel.title = getCurrentTitle();
            playlistPanel.loadPlaylist(getCurrentId());
        }
        onCompleted: {
            db.createPlaylistIndex();
            loadPlaylistIndexByTitle(false)
        }
        onTitleClicked: {
            if (cSort != 0){
                loadPlaylistIndexByTitle(false);
                cSort = 0;
            }
            else{
                loadPlaylistIndexByTitle(true);
                cSort = 1;
            }
        }
        onAuthorClicked: {
            if (cSort != 2){
                loadPlaylistIndexByAuthor(false);
                cSort = 2;
            }
            else{
                loadPlaylistIndexByAuthor(true);
                cSort = 3;
            }
        }
        onDurationClicked: {
            if (cSort != 4){
                loadPlaylistIndexByDuration(false);
                cSort = 4;
            }
            else{
                loadPlaylistIndexByDuration(true);
                cSort = 5;
            }
        }
    }

    /**************************************************************
     ADD / REMOVE BUTTONS
     **************************************************************/
    Item {
        id: addRemoveBtns;
        x: 4
        y: parent.height + 58
        visible: active && root.height > 275

        ImageButton { x: 25; y: 6; imageSource: "../images/item-remove.png"
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
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: {
            removeCurrentPlaylistIndex();
        }
        Component.onCompleted: visible = false;
    }

    /**************************************************************
     IMPORT PLAYLIST INDEX
     **************************************************************/
    function importPlaylistIndex(filepath){
        clear();
        var arr = plist.parse(filepath);
        for(var i=0; i<arr.length; i+=5){
                playlistIndex.model.append(
                        {"duration":    parseFloat(arr[i]),
                         "author":      arr[i+1],
                         "title":       arr[i+2],
                         "file":        arr[i+3],
                         "mime":        arr[i+4]  });
        }
    }

    /**************************************************************
     EXPORT PLAYLIST INDEX
     **************************************************************/
    function exportPlaylistIndex(filepath){
        var arr = [];
        var index = 0;
        for(var i = 0; i < playlistIndex.model.count; i++){
            arr[index] = playlistIndex.model.get(i).duration;
            arr[index+1] = playlistIndex.model.get(i).author;
            arr[index+2] = playlistIndex.model.get(i).title;
            arr[index+3] = playlistIndex.model.get(i).file;
            index = index+4;
        }
        plist.save(filepath, arr);
    }

    /**************************************************************
     LOAD PLAYLIST INDEX BY TITLE
     **************************************************************/
    function loadPlaylistIndexByTitle(desc){
        loadPlaylistIndex(db.selectPlaylistIndexOrderByTitle(desc));
    }

    /**************************************************************
     LOAD PLAYLIST INDEX BY AUTHOR
     **************************************************************/
    function loadPlaylistIndexByAuthor(desc){
        loadPlaylistIndex(db.selectPlaylistIndexOrderByAuthor(desc));
    }

    /**************************************************************
     LOAD PLAYLIST INDEX BY DURATION
     **************************************************************/
    function loadPlaylistIndexByDuration(desc){
        loadPlaylistIndex(db.selectPlaylistIndexOrderByDuration(desc));
    }

    /**************************************************************
     LOAD PLAYLIST INDEX
     **************************************************************/
    function loadPlaylistIndex(rows){
        playlistIndex.model.clear();
        var title = "";
        var author = "";
        for(var i=0; i < rows.length; i++) {
            title = rows.item(i).title === null ? "" : rows.item(i).title;
            author = rows.item(i).author === null ? "" : rows.item(i).author;
            playlistIndex.model.append({
                 "id":         rows.item(i).id,
                 "title":       title,
                 "author":      author,
                 "duration":    rows.item(i).duration,
                 "held":        false
                });
        }
    }

    /**************************************************************
     OVERWRITE OBJECT
     **************************************************************/
    QtObject {
         id: overwriteObject
         property int id
         property string author
         property real duration
    }

    /**************************************************************
     SAVE WARNING DIALOG
     **************************************************************/
    MessageDialog {
        id: saveWarningDialog
        title: "Save Playlist"
        text: "A playlist with this title already exists.\nDo you want to overwrite it?"
        onVisibleChanged: {
            standardButtons = StandardButton.Yes | StandardButton.No
        }
        onYes: {
            overwritePlaylistIndex();
        }
        Component.onCompleted: visible = false;
    }

    /**************************************************************
     SAVE PLAYLIST INDEX
     **************************************************************/
    function savePlaylistIndex(){
        var dur = playlistPanel.sumPlaylistDuration();
        var rows = db.selectPlaylistIndexByTitle(playlistPanel.title);
        if (rows && rows.length){
            var item = rows.item(0);
            overwriteObject.id = item.id;
            overwriteObject.author = item.author ? item.author : "----"
            overwriteObject.duration = dur;
            saveWarningDialog.open();
        }
        else{
            var nid = db.insertPlaylistIndexItem(playlistPanel.title, "", dur);
            playlistPanel.savePlaylist(nid);
            loadSort();
        }
    }

    /**************************************************************
     OVERWRITE PLAYLIST INDEX
     **************************************************************/
    function overwritePlaylistIndex(){
        db.updatePlaylistIndexItem(overwriteObject.id, playlistPanel.title, overwriteObject.author, overwriteObject.duration)
        db.deletePlaylistSet(overwriteObject.id);
        playlistPanel.savePlaylist(overwriteObject.id);
        loadSort();
    }

    /**************************************************************
     REMOVE PLAYLIST INDEX ITEM
     **************************************************************/
    function removeCurrentPlaylistIndex(){
        db.deletePlaylistIndexItem(getCurrentId());
        db.deletePlaylistSet(getCurrentId());
        playlistIndex.removeCurrentIndex();
    }

    /**************************************************************
     UDPATE CURRENT ITEM
     **************************************************************/
    function updateCurrentItem(){
        db.updatePlaylistIndexItem(
                             getCurrentId(),
                             getCurrentTitle(),
                             getCurrentAuthor(),
                             getCurrentDuration());
    }

    /**************************************************************
     GET CURRENT TITLE
     **************************************************************/
    function getCurrentTitle(){
        return playlistIndex.getCurrentTitle();
    }

    /**************************************************************
     GET CURRENT AUTHOR
     **************************************************************/
    function getCurrentAuthor() {
        return playlistIndex.getCurrentAuthor();
    }

    /**************************************************************
     GET CURRENT DURATION
     **************************************************************/
    function getCurrentDuration() {
        return playlistIndex.getCurrentDuration();
    }

    /**************************************************************
     GET CURRENT ID
     **************************************************************/
    function getCurrentId() {
        return playlistIndex.getCurrentId();
    }

    /**************************************************************
     SET INTERACTIVE
     **************************************************************/
    function setInteractive(value){
        playlistIndex.list.interactive = value;
    }

    /**************************************************************
     SET CURRENT INDEX
     **************************************************************/
    function setCurrentIndex(value){
        playlistIndex.list.currentIndex = value;
    }

    /**************************************************************
     GET LIST COUNT
     **************************************************************/
    function getCount(value){
        return playlistIndex.list.count;
    }

    /**************************************************************
     SET LIST PROPERTY
     **************************************************************/
    function setProperty(index, prop, value) {
        playlistIndex.setProperty(index, prop, value)
    }

    /**************************************************************
     GET CURRENT INDEX
     **************************************************************/
    function getCurrentIndex(){
        return playlistIndex.getCurrentIndex();
    }

    /**************************************************************
     HAS NEXT
     **************************************************************/
    function hasNext(){
        return playlistIndex.hasNext();
    }

    /**************************************************************
     HAS PREVIOUS
     **************************************************************/
    function hasPrevious(){
        return playlistIndex.hasPrevious();
    }

    /**************************************************************
     PREVIOUS INDEX
     **************************************************************/
    function previousIndex() {
        return playlistIndex.prevIndex();
    }

    /**************************************************************
     NEXT INDEX
     **************************************************************/
    function nextIndex() {
        playlistIndex.nextIndex();
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
        switch (cSort){
            case 0: loadPlaylistIndexByTitle(false);      break;
            case 1: loadPlaylistIndexByTitle(true);       break;
            case 2: loadPlaylistIndexByAuthor(false);     break;
            case 3: loadPlaylistIndexByAuthor(true);      break;
            case 4: loadPlaylistIndexByDuration(false);   break;
            case 5: loadPlaylistIndexByDuration(true);    break;
        }
    }

    /**************************************************************
     RETURNS TITLE FORMATTED FOR CONTROL PANEL
     **************************************************************/
    function clear() {
        return playlistIndex.clear();
    }

    /**************************************************************
     REMOVE ITEM
     **************************************************************/
    function remove() {
        if (getCount() && getCurrentIndex() >= 0){
            removeWarningDialog.text = "Do you want to remove '"+ getCurrentTitle() +
               "'\nfrom the playlists?"
            removeWarningDialog.open();
        }
    }

}
