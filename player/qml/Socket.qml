import QtQuick 2.2
import QtWebSockets 1.0

/**************************************************************
 SOCKET
 **************************************************************/
Item {
    id: root
    property string host: "localhost"
    property real sendPort: 3334;
    property real receivePort: 3333;
    property string address: "";
    property  real seekValue: -1;


    /**************************************************************
     WEB SOCKET RECEIVE
     **************************************************************/
    WebSocket {
        id: webSocket
        active: true
        url :"ws://localhost:9092/"
        property real duration: 0;



        onTextMessageReceived: {


            var m = JSON.parse(message);

            console.log(message);

            if (m.address == "/input/position/"){




                    if (controlPanel.cType == "video") {
                        if (m.message == "-inf"){
                            return
                        }
                       if (Number.fromLocaleString(m.message) === null)
                            controlPanel.positionValue = 0;
                       else
                           controlPanel.positionValue = Number.fromLocaleString (m.message);

                        controlPanel.time = secondsToHms(controlPanel.cDuration* Number.fromLocaleString (m.message));

                       if (controlPanel.positionValue*controlPanel.duration >= controlPanel.duration){
                            playlistPanel.next();
                       }
                    }
            }
            else if (m.address == "/input/duration/") {


                var file = m.message.split(",")[0];
                var dur = parseFloat(m.message.split(",")[1]);
                controlPanel.cDuration = parseFloat(dur);

                db.updateLibraryItemDuration(file, dur)
                libraryPanel.loadSort();

                var c;
                for (var i=0; i<playlistPanel.getCount(); i++){
                    c = playlistPanel.getFileByIndex(i);

                    if (c === plist.convertToNativePath(file)){
                        playlistPanel.setProperty(i, "duration", dur);
                        playlistPanel.sumPlaylistDuration();
                    }
                }
            }
            else if (m.address == "/input/") {

                if  (m.message == "end") {
                    controlPanel.end();
                }
            }
        }
    }



    /**************************************************************
     SEND FUNCTIONS
     **************************************************************/

    function send(msg){
       webSocket.sendTextMessage(msg);
    }

    function sendExit(){
        send(JSON.stringify("/exit/"));
    }

    // input
    function sendSource(value){
        var msg = { address: "/input/source/", message: value };
        send(JSON.stringify(msg));
    }

    function sendFile(filepath){
        //send("/input/file/", filepath);
        var msg = { address: "/input/file/", message: filepath };
        send(JSON.stringify(msg))
    }

    function sendPlay(){
        //send("/input/", "play");
        var msg = { address: "/input/", message: "play" };
        send(JSON.stringify(msg))
    }

    function sendPause(){
       //send("/input/", "stop");
        var msg = { address: "/input/", message: "stop" };
        send(JSON.stringify(msg))
    }

    function sendSeek(pos){
       seekValue = pos;
       //send("/input/seek/", pos);
        var msg = { address: "/input/seek/", message: pos };
        send(JSON.stringify(msg))
    }

    function sendVolume(vol){
      //send("/input/volume/", vol);
        var msg = { address: "/input/volume/", message: vol };
        send(JSON.stringify(msg))
    }

    function sendLoop(value){
        //send("/input/loop/", value);
        var msg = { address: "/input/loop/", message: value };
        send(JSON.stringify(msg))
    }

    function sendFormat(value){
       //send("/input/format/", value);
        var msg = { address: "/input/format/", message: value };
        send(JSON.stringify(msg))
    }
    function sendFlip(value){
       //send("/input/flip/", value);
        var msg = { address: "/input/flip/", message: value };
        send(JSON.stringify(msg))
    }
    function sendRotate(value){
       //send("/input/rotate/", value);
        var msg = { address: "/input/rotate/", message: value };
        send(JSON.stringify(msg))
    }
    function sendScale(value){
       //send("/input/scale/", value);
        var msg = { address: "/input/scale/", message: value };
        send(JSON.stringify(msg))
    }
    function sendTilt(value){
       //send("/input/tilt/", value);
        var msg = { address: "/input/tilt/", message: value };
        send(JSON.stringify(msg))
    }

    // projector
    function sendProjectorMenu(value){
        //send("/projector/menu/", value)
        var msg = { address: "/projector/menu/", message: value };
       send(JSON.stringify(msg));

    }
    function sendProjectorEnable(value){
        //send("/projector/enable/", value)
        var msg = { address: "/projector/enable/", message: value };
        send(JSON.stringify(msg))
    }
    function sendProjectorPolar(value){
        //send("/projector/polar/", value)
        var msg = { address: "/projector/polar/", message: value };
        send(JSON.stringify(msg))
    }

    // focus
    function sendAppFocus(value){
        send("/projector/focus/", value)
        var msg = { address: "/projector/focus/", message: value };
        send(JSON.stringify(msg))
    }
}
