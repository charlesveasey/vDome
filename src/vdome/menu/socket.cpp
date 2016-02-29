#include "socket.h"
namespace vd {
    
//--------------------------------------------------------------
Socket::Socket(){
    enabled = true;
    host = "localhost";
    send = 3333;
    receive = 3334;
}

//--------------------------------------------------------------
void Socket::setup(){
    oscSender.setup(host,send);
    oscReceiver.setup(receive);
}
    
//--------------------------------------------------------------
void Socket::update(){}
    
//--------------------------------------------------------------
void Socket::sendDuration(){
    sMsg.clear();
    sMsg.setAddress("/input/duration");
    sMsg.addStringArg(input->getFilepath() + "," + ofToString( input->getDuration() ));
    oscSender.sendMessage(sMsg);    
}
    
//--------------------------------------------------------------
void Socket::sendEnd(){
    sMsg.clear();
    sMsg.setAddress("/input/");
    sMsg.addStringArg("end");
    oscSender.sendMessage(sMsg);
}
    
//--------------------------------------------------------------
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

//--------------------------------------------------------------
void Socket::saveXML(ofXml &xml) {
    xml.setTo("socket");

    if (enabled) xml.setAttribute("enabled", "on");
    else         xml.setAttribute("enabled", "off");
    
    xml.setAttribute("host", ofToString(host));
    xml.setAttribute("send", ofToString(send));
    xml.setAttribute("receive", ofToString(receive));

    xml.setToParent();
}


}//////////////
