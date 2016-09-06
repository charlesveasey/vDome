#pragma once
#include "ofMain.h"
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
	void    sendEnd();
    void    loadXML(ofXml &xml);
    void    saveXML(ofXml &xml);
    
    string  host;
    int     send;
    int     receive;
    bool    enabled;
    Input   *input;
	Model	*model;

	float			lastInputPosition;
	bool			hasEnded;
    
};

}
