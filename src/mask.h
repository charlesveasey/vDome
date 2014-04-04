#pragma once

#include "ofMain.h"

class Mask {
public:
    Mask();
    
    void draw();
    
    void mousePressed(ofMouseEventArgs& mouseArgs);
    void mouseDragged(ofMouseEventArgs& mouseArgs);
    void mouseReleased(ofMouseEventArgs& mouseArgs);
   
    void keyPressed(int key);
    void keyReleased(int key);

    ofImage brushImage;
    ofMesh brush;
    ofFbo maskFbo;

    float brushOpacity;
    float brushScale;

private:
    bool mouseDown;
    
    int mouseX;
    int mouseY;
    
    int width;
    int height;
    
    int brushWidth;
    int brushHeight;

    bool erase;
};
