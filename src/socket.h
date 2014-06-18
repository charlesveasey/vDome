#pragma once
#include "ofMain.h"
#include "ofxOsc.h"
#include "input.h"
namespace vd {

class Socket {

public:
    Socket();
    void setup();
    void update();

    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);
    
    int port;
    bool enabled;
    
    Input *input;

private:
    ofxOscReceiver oscReceiver;

};

}
