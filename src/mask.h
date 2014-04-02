#pragma once

#include "ofMain.h"

class Mask {
public:
    Mask();
    
    void draw();
    
    void mousePressed(ofMouseEventArgs& mouseArgs);
    void mouseDragged(ofMouseEventArgs& mouseArgs);
    void mouseReleased(ofMouseEventArgs& mouseArgs);
   
    ofImage brushImage;
    ofMesh brush;
    ofFbo maskFbo;
        
private:
    bool mouseDown;
    
    int mouseX;
    int mouseY;
    
    int width;
    int height;
    
    int brushImageSize;
    int brushImageAlpha;

};
