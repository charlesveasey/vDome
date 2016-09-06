import QtQuick 2.0
import QtQuick.LocalStorage 2.0

Item {


    /**************************************************************
     OPEN DATABASE
     **************************************************************/
    function openDB(){
        return LocalStorage.openDatabaseSync("vDome", "1.0", "vDome Database", 1000000);
    }








    /**************************************************************
     CREATE LIBRARY TABLE
     **************************************************************/
    function createLibrary(){
        var db = openDB();
        db.transaction(
            function(tx) {
                tx.executeSql('CREATE TABLE IF NOT EXISTS library(title TEXT, author TEXT, duration REAL, file TEXT, mime TEXT, timestamp REAL)');
             })
    }

    /**************************************************************
     DROP LIBRARY TABLE
     **************************************************************/
    function dropLibrary(){
        var db = openDB();
        db.transaction(
            function(tx) {
                tx.executeSql('DROP TABLE library');
             })
    }

    /**************************************************************
     PRINT LIBRARY TABLE
     **************************************************************/
    function printLibrary() {
        var rows = selectLibrary();
        var r = ""
        for(var i = 0; i < rows.length; i++) {
                    r += rows.item(i).title + ", " +
                    rows.item(i).author + ", " +
                    rows.item(i).duration + ", " +
                    rows.item(i).file + ", " +
                    rows.item(i).mime + ", " +
                    rows.item(i).timestamp + "\n"
        }
        console.log(r);
    }

    /**************************************************************
     INSERT LIBRARY ITEM
     **************************************************************/
    function insertLibraryItem(title, author, duration, file, mime) {
        var db = openDB();
        db.transaction(
            function(tx) {
                tx.executeSql('INSERT INTO library VALUES(?,?,?,?,?,?)', [ title, author, duration, file, mime, new Date().getTime()  ]);
            })
    }

    /**************************************************************
     UDPATE LIBRARY ITEM
     **************************************************************/
    function updateLibraryItem(title, author, duration, file, mime) {
        file = plist.convertToNativePath(file);

        var db = openDB();
        db.transaction(
            function(tx) {
                tx.executeSql('UPDATE library SET title = "'      + title + '", ' +
                                                  'author = "'    + author + '", ' +
                                                  'duration = '   + duration + ', ' +
                                                  'file = "'      + file + '", ' +
                                                  'mime = "'      + mime + '", ' +
                                                  'timestamp = '  + new Date().getTime() + ' ' +
                              'WHERE file = "' + file + '"');
            })
    }

    /**************************************************************
     UDPATE LIBRARY ITEM DURATION
     **************************************************************/
    function updateLibraryItemDuration(file, duration) {
        file = plist.convertToNativePath(file);

        var db = openDB();
        db.transaction(
            function(tx) {

               tx.executeSql('UPDATE library SET duration = '   + duration + ', '+
                                                  'timestamp = '  + new Date().getTime() + ' ' +
                              'WHERE file = "' + file + '"');
            })
    }

    /**************************************************************
     DELETE LIBRARY ITEM
     **************************************************************/
    function deleteLibraryItem(file) {
        var db = openDB();
        db.transaction(
            function(tx) {
                tx.executeSql('DELETE FROM library WHERE file="'+file+'"');
            })
    }

    /**************************************************************
     SELECT ALL FROM LIBRARY TABLE
     **************************************************************/
    function selectLibrary() {
        var db = openDB();
        var rows;
        db.transaction(
            function(tx) {
                var res = tx.executeSql('SELECT * FROM library');
                rows = res.rows;
            })
        return rows;
    }


    /**************************************************************
     SELECT ALL FROM LIBRARY TABLE, ORDER BY TITLE DESC / ASC
     **************************************************************/
    function selectLibraryOrderByTitle(desc) {
        var db = openDB();
        var rows;
        var order = desc ? "DESC" : "ASC";
        db.transaction(
            function(tx) {
                var res = tx.executeSql('SELECT * FROM library ORDER BY title ' + order );
                rows = res.rows;
            })
        return rows;
    }

    /**************************************************************
     SELECT ALL FROM LIBRARY TABLE, ORDER BY AUTHOR DESC / ASC
     **************************************************************/
    function selectLibraryOrderByAuthor(desc) {
        var db = openDB();
        var rows;
        var order = desc ? "DESC" : "ASC";
        db.transaction(
            function(tx) {
                var res = tx.executeSql('SELECT * FROM library ORDER BY author ' + order + ', title ' + order );
                rows = res.rows;
            })
        return rows;
    }

    /**************************************************************
     SELECT ALL FROM LIBRARY TABLE, ORDER BY DURATION DESC / ASC
     **************************************************************/
    function selectLibraryOrderByDuration(desc) {
        var db = openDB();
        var rows;
        var order = desc ? "DESC" : "ASC";
        db.transaction(
            function(tx) {
                var res = tx.executeSql('SELECT * FROM library ORDER BY duration ' + order + ', title ' + order   );
                rows = res.rows;
            })
        return rows;
    }


    /**************************************************************
     SEARCH LIBRARY BY FILE:
        return true if found
     **************************************************************/
    function searchLibraryByFile(file) {
        var db = openDB();
        var rows;
        db.transaction(
            function(tx) {
                var res = tx.executeSql('SELECT file FROM library WHERE file = "'+file +'"');
                rows = res.rows;
            })

        if (rows.length)
            return true;
        else
            return false;
    }








    /**************************************************************
     DROP PLAYLIST INDEX TABLE
     **************************************************************/
    function dropPlaylistIndex(){
        var db = openDB();
        db.transaction(
            function(tx) {
                tx.executeSql('DROP TABLE playlistIndex');
             })
    }

    /**************************************************************
     CREATE PLAYLIST INDEX TABLE
     **************************************************************/
    function createPlaylistIndex(){
        var db = openDB();
        db.transaction(
            function(tx) {
                tx.executeSql('CREATE TABLE IF NOT EXISTS playlistIndex(id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT, author TEXT, duration REAL, timestamp REAL)');
             })
    }

    /**************************************************************
     PRINT PLAYLIST INDEX TABLE
     **************************************************************/
    function printPlaylistIndex() {
        var rows = selectPlaylistIndex();
        var r = ""
        for(var i = 0; i < rows.length; i++) {
            r +=    rows.item(i).id + ", " +
                    rows.item(i).title + ", " +
                    rows.item(i).author + ", " +
                    rows.item(i).duration + ", " +
                    rows.item(i).timestamp + "\n"
        }
        console.log(r);
    }

    /**************************************************************
     INSERT PLAYLIST INDEX ITEM
     **************************************************************/
    function insertPlaylistIndexItem(title, author, duration, timestamp) {
        var db = openDB();
        var res;
        db.transaction(
            function(tx) {
                res = tx.executeSql('INSERT INTO playlistIndex VALUES(?,?,?,?,?)', [ null, title, author, duration, new Date().getTime() ]);
            })
        return res.insertId;
    }

    /**************************************************************
     UDPATE PLAYLIST INDEX ITEM
     **************************************************************/
    function updatePlaylistIndexItem(id, title, author, duration) {
        var db = openDB();
        db.transaction(
            function(tx) {
                tx.executeSql('UPDATE playlistIndex SET title = "'+ title + '", ' +
                                                  'author = "'    + author + '", ' +
                                                  'duration = '   + duration + ', ' +
                                                  'timestamp = '  + new Date().getTime() + " " +
                              'WHERE id = "' + id + '"');
            })
    }

    /**************************************************************
     DELETE PLAYLIST INDEX ITEM
     **************************************************************/
    function deletePlaylistIndexItem(id) {
        var db = openDB();
        db.transaction(
            function(tx) {
                tx.executeSql('DELETE FROM playlistIndex WHERE id = '+id);
            }
        )
    }

    /**************************************************************
     SELECT ALL FROM PLAYLIST INDEX TABLE
     **************************************************************/
    function selectPlaylistIndex() {
        var db = openDB();
        var rows;
        db.transaction(
            function(tx) {
                var res = tx.executeSql('SELECT * FROM playlistIndex');
                rows = res.rows;

            })
        return rows;
    }


    /**************************************************************
     SELECT PLAYLIST INDEX BY TITLE
     **************************************************************/
    function selectPlaylistIndexByTitle(title) {
        var db = openDB();
        var rows;
        db.transaction(
            function(tx) {
                var res = tx.executeSql('SELECT * FROM playlistIndex WHERE title = "'+title +'"');
                rows = res.rows;
            })
         return rows;
    }


    /**************************************************************
     SELECT ALL FROM PLAYLIST INDEX TABLE, ORDER BY TITLE DESC / ASC
     **************************************************************/
    function selectPlaylistIndexOrderByTitle(desc) {
        var db = openDB();
        var rows;
        var order = desc ? "DESC" : "ASC";
        db.transaction(
            function(tx) {
                var res = tx.executeSql('SELECT * FROM playlistIndex ORDER BY title ' + order );
                rows = res.rows;
            })
        return rows;
    }

    /**************************************************************
     SELECT ALL FROM PLAYLIST INDEX TABLE, ORDER BY AUTHOR DESC / ASC
     **************************************************************/
    function selectPlaylistIndexOrderByAuthor(desc) {
        var db = openDB();
        var rows;
        var order = desc ? "DESC" : "ASC";
        db.transaction(
            function(tx) {
                var res = tx.executeSql('SELECT * FROM playlistIndex ORDER BY author ' + order + ', title ' + order );
                rows = res.rows;
            })
        return rows;
    }

    /**************************************************************
     SELECT ALL FROM PLAYLIST INDEX TABLE, ORDER BY DURATION DESC / ASC
     **************************************************************/
    function selectPlaylistIndexOrderByDuration(desc) {
        var db = openDB();
        var rows;
        var order = desc ? "DESC" : "ASC";
        db.transaction(
            function(tx) {
                var res = tx.executeSql('SELECT * FROM playlistIndex ORDER BY duration ' + order + ', title ' + order   );
                rows = res.rows;
            })
        return rows;
    }







    /**************************************************************
     CREATE PLAYLIST TABLE
     **************************************************************/
    function createPlaylist(){
        var db = openDB();
        db.transaction(
            function(tx) {
                tx.executeSql('CREATE TABLE IF NOT EXISTS playlist(id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT,
                                    author TEXT, duration REAL, file TEXT, mime TEXT, timestamp REAL, sequence INTEGER, indexId INTEGER)');
             })
    }

    /**************************************************************
     DROP PLAYLIST TABLE
     **************************************************************/
    function dropPlaylist(){
        var db = openDB();
        db.transaction(
            function(tx) {
                tx.executeSql('DROP TABLE playlist');
             })
    }

    /**************************************************************
     PRINT PLAYLIST TABLE
     **************************************************************/
    function printPlaylist() {
        var rows = selectPlaylist();
        var r = ""
        for(var i = 0; i < rows.length; i++) {
            r +=    rows.item(i).id + ", " +
                    rows.item(i).title + ", " +
                    rows.item(i).author + ", " +
                    rows.item(i).duration + ", " +
                    rows.item(i).file + ", " +
                    rows.item(i).mime + ", " +
                    rows.item(i).timestamp + ", " +
                    rows.item(i).sequence + ", " +
                    rows.item(i).indexId + "\n"
        }
        console.log(r);
    }

    /**************************************************************
     INSERT PLAYLIST ITEM
     **************************************************************/
    function insertPlaylistItem(indexId, title, author, duration, file, mime, sequence) {
        var db = openDB();
        db.transaction(
            function(tx) {
                tx.executeSql('INSERT INTO playlist VALUES(?,?,?,?,?,?,?,?,?)',
                              [ null, title, author, duration, file, mime, new Date().getTime(), sequence, indexId ]);
            })
    }

    /**************************************************************
     DELETE PLAYLIST SET
     **************************************************************/
    function deletePlaylistSet(indexId) {
        var db = openDB();
        db.transaction(
            function(tx) {
                tx.executeSql('DELETE FROM playlist WHERE indexId='+indexId);
            })
    }

    /**************************************************************
     SELECT ALL FROM PLAYLIST TABLE
     **************************************************************/
    function selectPlaylist() {
        var db = openDB();
        var rows;
        db.transaction(
            function(tx) {
                var res = tx.executeSql('SELECT * FROM playlist');
                rows = res.rows;
            })
        return rows;
    }

    /**************************************************************
     SELECT PLAYLIST FROM PLAYLIST TABLE BY INDEX ID
     **************************************************************/
    function selectPlaylistByIndexId(indexId) {
        var db = openDB();
        var rows;
        db.transaction(
            function(tx) {
                var res = tx.executeSql('SELECT * FROM playlist WHERE indexId = '+indexId +
                                        ' ORDER BY sequence');
                rows = res.rows;
            })
        return rows;
    }












    /**************************************************************
     CREATE SETTINGS TABLE
     **************************************************************/
    function createSettings(){
        var db = openDB();
        db.transaction(
            function(tx) {
               var res = tx.executeSql('CREATE TABLE IF NOT EXISTS settings( librarySlide REAL,
                                                                    socketHost TEXT,
                                                                    socketSend INTEGER,
                                                                    socketReceive INTEGER,
                                                                    startupLaunch INTEGER,
                                                                    startupFilepath TEXT,
                                                                    timestamp REAL)');
             })


        var rows = selectSettings();
        if (!rows.length){
            db.transaction(
                function(tx) {
                    tx.executeSql('INSERT INTO settings VALUES(?,?,?,?,?,?,?)',
                                                                        [ 10,
                                                                        "localhost",
                                                                        3334,
                                                                        3333,
                                                                        false,
                                                                        null,
                                                                        new Date().getTime() ]);
                 })
        }

    }

    /**************************************************************
     DROP SETTINGS TABLE
     **************************************************************/
    function dropSettings(){
        var db = openDB();
        db.transaction(
            function(tx) {
                tx.executeSql('DROP TABLE settings');
             })
    }

    /**************************************************************
     PRINT SETTINGS TABLE
     **************************************************************/
    function printSettings() {
        var rows = selectSettings();
        var r = ""
        for(var i = 0; i < rows.length; i++) {
                    r += rows.item(i).librarySlide + ", " +
                    rows.item(i).socketHost + ", " +
                    rows.item(i).socketSend + ", " +
                    rows.item(i).socketReceive + ", " +
                    rows.item(i).startupLaunch + ", " +
                    rows.item(i).startupFilepath + "\n"
        }
        console.log(r);
    }


    /**************************************************************
     SELECT ALL FROM LIBRARY TABLE
     **************************************************************/
    function selectSettings() {
        var db = openDB();
        var rows;
        db.transaction(
            function(tx) {
                var res = tx.executeSql('SELECT * FROM settings');
                rows = res.rows;
            })
        return rows;
    }



    /**************************************************************
     UPDATE SETTINGS ITEM
     **************************************************************/
    function updateSettings(item) {
        var db = openDB();
        db.transaction(
            function(tx) {
                tx.executeSql('UPDATE settings SET librarySlide = '     + item.librarySlide         + ', ' +
                                                  'socketHost = "'       + item.socketHost           + '", ' +
                                                  'socketSend = '        + item.socketSend           + ', ' +
                                                  'socketReceive = '    + item.socketReceive        + ', ' +
                                                  'startupLaunch = '    + item.startupLaunch        + ', ' +
                                                  'startupFilepath = "'  + item.startupFilepath      + '", ' +
                                                  'timestamp = '         +  new Date().getTime() + ' ' +
                              'WHERE timestamp = "' + item.timestamp + '"');
            })
    }








    /**************************************************************
     DEBUG
     **************************************************************/
    function test(){
        //printPlaylist()
        //dropLibrary();
        //dropPlaylist();
        //dropPlaylistIndex();

        //createLibrary()

        //insertLibraryItem("Test Title", "Test Author", 3.45553, "filename.png", "image");

        //selectLibrary();

        //printPlaylist();
        //printPlaylistIndex();
        //printLibrary();

        //dropSettings();
        //createSettings();


        //printSettings();
        //updateSettings();
    }


    Component.onCompleted: {
        test();
    }



}

