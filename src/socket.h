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

    ofxOscReceiver oscReceiver;
    
    int port;
    bool enabled;
    
    Input *input;
};

}
