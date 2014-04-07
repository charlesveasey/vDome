#pragma once

#include "ofMain.h"

class Mask {
public:
    Mask();
    
    void setup();
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
    void save(int i);
    void load(int i);
    void reset();
    

private:
    bool mouseDown;
    
    int mouseX;
    int mouseY;
    
    int width;
    int height;
    
    int brushWidth;
    int brushHeight;

    bool erase;
    
    ofImage maskFboImage;
    string filename;
    int indx;
};
