#include "tcp.h"

void Tcp::init(){
    enabled = true;
    port = 11999;
}

void Tcp::setup(){
    
    if (!enabled) return;
    
	//setup the server to listen on 11999
	server.setup(port);
	//optionally set the delimiter to something else.  The delimter in the client and the server have to be the same, default being [/tcp]
	server.setMessageDelimiter("\n");
}

void Tcp::loadXML(ofXml &xml) {
    if (xml.exists("tcp[@enabled]"))
        enabled = ofToBool( xml.getAttribute("tcp[@enabled]") );
    if (xml.exists("tcp[@port]"))
        port = ofToInt( xml.getAttribute("tcp[@port]") );
    setup();
}

void Tcp::saveXML(ofXml &xml) {
    xml.setAttribute("port[@enabled]", ofToString(enabled));
    xml.setAttribute("port[@port]", ofToString(enabled));
    
	//for each client lets send them a message letting them know what port they are connected on
	for(int i = 0; i < server.getLastID(); i++){
		if( !server.isClientConnected(i) )continue;
		server.send(i, "hello client - you are connected on port - "+ofToString(server.getClientPort(i)) );
	}    
}


void Tcp::draw() {
        
    
    ofSetHexColor(0xFFFFFF);
    ofDrawBitmapString("Port: "+ofToString(server.getPort()), x, y);
                       
                       
	//for each connected client lets get the data being sent and lets print it to the screen
	for(unsigned int i = 0; i < (unsigned int)server.getLastID(); i++){
        
		if( !server.isClientConnected(i) )continue;
        
		//get the ip and port of the client
		string port = ofToString( server.getClientPort(i) );
		string ip   = server.getClientIP(i);
		string info = "client "+ofToString(i)+" -connected from "+ip+" on port: "+port;
        
        
		//if we don't have a string allocated yet
		//lets create one
		if(i >= storeText.size() ){
			storeText.push_back( string() );
		}
        
		//we only want to update the text we have recieved there is data
		string str = server.receive(i);
        
		if(str.length() > 0){
			storeText[i] = str;
		}
        
		//draw the info text and the received text bellow it
		ofDrawBitmapString(info, x, y+20);
		ofDrawBitmapString(storeText[i], x, y + 40);
        
	}
}
