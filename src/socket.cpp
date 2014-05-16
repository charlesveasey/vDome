#include "socket.h"
namespace vd {

/******************************************

 CONSTRUCTOR

 ********************************************/

Socket::Socket(){
    enabled = true;
    port = 12345;
}

/******************************************

 SETUP

 ********************************************/

void Socket::setup(){
	oscReceiver.setup(port);
}
    
/******************************************
 
 UPDATE
 
 ********************************************/

void Socket::update(){
    while(oscReceiver.hasWaitingMessages()){
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
        
		if (m.getAddress() == "/input/"){
            if      (m.getArgAsString(0) == "play")         input->play();
            else if (m.getArgAsString(0) == "stop")         input->stop();
            else if (m.getArgAsString(0) == "toggle")       input->toggle();
            else if (m.getArgAsString(0) == "prev")         input->prev();
            else if (m.getArgAsString(0) == "next")         input->next();
            else if (m.getArgAsString(0) == "prevFrame")    input->prevFrame();
            else if (m.getArgAsString(0) == "nextFrame")    input->nextFrame();
		}
		else if (m.getAddress() == "/input/loop/") {
            if (m.getArgAsString(0) == "on") input->setLoop(true);
            else                             input->setLoop(false);
		}
        else if (m.getAddress() == "/input/seek/") {
            input->seek(m.getArgAsFloat(0));
		}
        else if (m.getAddress() == "/input/source/") {
            input->setSource(m.getArgAsString(0));
		}
        else if (m.getAddress() == "input/media/") {
            input->setFile(m.getArgAsString(0));
		}
	}
}

/******************************************

 SETTINGS

 ********************************************/

void Socket::loadXML(ofXml &xml) {
    if (xml.exists("socket[@enabled]")) {
        string str = ofToString(xml.getAttribute("socket[@enabled]"));
        if (str == "on")
            enabled = true;
        else
            enabled = false;
    }
    if (xml.exists("socket[@port]"))
        port = ofToInt( xml.getAttribute("socket[@port]") );
    
    if (enabled)
        setup();
}

void Socket::saveXML(ofXml &xml) {
    xml.setTo("socket");

    if (enabled) xml.setAttribute("enabled", "on");
    else         xml.setAttribute("enabled", "off");
    
    xml.setAttribute("port", ofToString(port));
    xml.setToParent();
}

}
