#pragma once
#include "ofMain.h"
#include "ofxOsc.h"
#include "input.h"
namespace vd {
class Input;
    
class Socket {

public:
    Socket();
    void setup();
    void update();
    
    void sendDuration();
    void sendEnd();
    
    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);
    
    string  host;
    int send;
    int receive;
    bool enabled;
    
    Input *input;

private:
    ofxOscSender oscSender;
    ofxOscReceiver oscReceiver;
    ofxOscMessage sMsg;
    ofxOscMessage rMsg;
};

}
