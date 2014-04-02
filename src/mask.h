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
    
    // mouse
    void mousePressed(ofMouseEventArgs& mouseArgs);
    void mouseDragged(ofMouseEventArgs& mouseArgs);
    void mouseReleased(ofMouseEventArgs& mouseArgs);
   
    ofImage brushImage;
    ofFbo maskFbo;
    ofFbo renderFbo;
    
    bool mouseDown;
    
    int mouseX;
    int mouseY;
    
    int width;
    int height;
};
