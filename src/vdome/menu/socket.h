#pragma once
#include "ofMain.h"
#include "ofxOsc.h"
#include "input.h"
#include "model.h"
namespace vd {
class Input;
class Model;

class Socket {

public:
    Socket();
    void    setup();
    void    update();
    void    sendDuration();
    void    sendEnd();
    void    loadXML(ofXml &xml);
    void    saveXML(ofXml &xml);
    
    string  host;
    int     send;
    int     receive;
    bool    enabled;
    Input   *input;
	Model	*model;

    static	ofEvent<int> sourceEvent;
    static	ofEvent<int> formatEvent;

private:
    ofxOscSender    oscSender;
    ofxOscReceiver  oscReceiver;
    ofxOscMessage   sMsg;
    ofxOscMessage   rMsg;

	float			lastInputPosition;
};

}
