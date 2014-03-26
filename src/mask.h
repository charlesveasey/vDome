#pragma once

#include "ofMain.h"

class Mask {
public:
    Mask();
    
	void setup();
	void update();
	void draw();
    
    void begin();
    void end();
    
    void onMousePressed(ofMouseEventArgs& mouseArgs);    
    void onMouseDragged(ofMouseEventArgs& mouseArgs);
    void onMouseReleased(ofMouseEventArgs& mouseArgs);

    ofImage brushImage;
    ofFbo maskFbo;
    ofFbo renderFbo;
    
    bool mouseDown;
    
    int mouseX;
    int mouseY;
    
    int width;
    int height;
};
