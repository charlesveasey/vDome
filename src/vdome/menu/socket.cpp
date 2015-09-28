#include "socket.h"
namespace vd {
ofEvent<int> Socket::sourceEvent = ofEvent<int>();
ofEvent<int> Socket::formatEvent = ofEvent<int>();

/******************************************

 CONSTRUCTOR

 ********************************************/

Socket::Socket(){
    enabled = true;
    host = "localhost";
    send = 3333;
    receive = 3334;
}

/******************************************

 SETUP

 ********************************************/

void Socket::setup(){
    oscSender.setup(host,send);
    oscReceiver.setup(receive);
}
    
/******************************************
 
 UPDATE
 
 ********************************************/

void Socket::update(){
    
    // receive
    while(oscReceiver.hasWaitingMessages()){
        rMsg.clear();
		oscReceiver.getNextMessage(&rMsg);
        
		if (rMsg.getAddress() == "/input/"){
            if (rMsg.getArgAsString(0) == "play")         
				input->play();
            else if (rMsg.getArgAsString(0) == "stop")         
				input->stop();
		}
		else if (rMsg.getAddress() == "/input/loop/") {
            if (rMsg.getArgAsString(0) == "on") 				
				input->setLoop(true);
            else                                				
				input->setLoop(false);
		}
        else if (rMsg.getAddress() == "/input/seek/") {
            input->seek(ofToFloat(rMsg.getArgAsString(0)));
		}
        else if (rMsg.getAddress() == "/input/source/") {
            int s = input->convertSourceString(rMsg.getArgAsString(0));
            ofNotifyEvent(sourceEvent,s,this);
		}
        else if (rMsg.getAddress() == "/input/file/") {
            ofFile oFile;
            oFile.open(rMsg.getArgAsString(0));
            string file = oFile.getAbsolutePath();
            input->openFile(file);
		}
        else if (rMsg.getAddress() == "/input/volume/") {
            input->setVolume(ofToFloat(rMsg.getArgAsString(0)));
		}
        else if (rMsg.getAddress() == "/input/format/") {
            int s = input->convertFormatString(rMsg.getArgAsString(0));
            ofNotifyEvent(formatEvent,s,this);
        }

		// transform
		else if (rMsg.getAddress() == "/input/flip/") {
            string s = rMsg.getArgAsString(0);

			if (rMsg.getArgAsString(0) == "on")
				model->setTextureFlip(true);
			else
				model->setTextureFlip(false);
		}
		else if (rMsg.getAddress() == "/input/scale/") {
			float f = ofToFloat(rMsg.getArgAsString(0));
			model->setTextureScale(f);
		}		
		else if (rMsg.getAddress() == "/input/rotate/") {
			float f = ofToFloat(rMsg.getArgAsString(0));
			model->setTextureRotate(f);
		}			
		else if (rMsg.getAddress() == "/input/tilt/") {
			float f = ofToFloat(rMsg.getArgAsString(0));
			model->setTextureTilt(f);
		}			
	}
    
    // send
    sMsg.clear();
    if (input->getSourceInt() == input->MEDIA) {
		if (lastInputPosition != input->getPosition()){
			sMsg.setAddress("/input/position");
			sMsg.addStringArg(ofToString( input->getPosition() ));
			oscSender.sendMessage(sMsg);
		}
		lastInputPosition = input->getPosition();
	}

}
    
    
/******************************************
 
 SEND
 
 ********************************************/

void Socket::sendDuration(){
    sMsg.clear();
    sMsg.setAddress("/input/duration");
    sMsg.addStringArg(input->getFilepath() + "," + ofToString( input->getDuration() ));
    oscSender.sendMessage(sMsg);    
}
    
void Socket::sendEnd(){
    sMsg.clear();
    sMsg.setAddress("/input/");
    sMsg.addStringArg("end");
    oscSender.sendMessage(sMsg);
}
    
/******************************************

 SETTINGS

 ********************************************/

void Socket::loadXML(ofXml &xml) {
    if (xml.exists("socket[@enabled]")) {
        string str = ofToString(xml.getAttribute("socket[@enabled]"));
        if (str == "on") enabled = true;
        else             enabled = false;
    }
    
    if (xml.exists("socket[@host]"))
        host = ofToString( xml.getAttribute("socket[@host]") );
    if (xml.exists("socket[@send]"))
        send = ofToInt( xml.getAttribute("socket[@send]") );
    if (xml.exists("socket[@receive]"))
        receive = ofToInt( xml.getAttribute("socket[@receive]") );
    
    if (enabled)
        setup();
}

void Socket::saveXML(ofXml &xml) {
    xml.setTo("socket");

    if (enabled) xml.setAttribute("enabled", "on");
    else         xml.setAttribute("enabled", "off");
    
    xml.setAttribute("host", ofToString(host));
    xml.setAttribute("send", ofToString(send));
    xml.setAttribute("receive", ofToString(receive));

    xml.setToParent();
}

}
