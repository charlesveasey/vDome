#pragma once
#include "ofMain.h"
namespace vd {

class Mask {
public:
    Mask();

    void    init(int i);
    void    setup();
    void    draw();
    void    save(int projectorStartingIndex);
    void    load(int projectorStartingIndex);
    int     store(int fIndex);
    void    recall(int fIndex);
    void    reset();

    void    mousePressed(ofMouseEventArgs& mouseArgs);
    void    mouseDragged(ofMouseEventArgs& mouseArgs);
    void    mouseReleased(ofMouseEventArgs& mouseArgs);

    void    keyPressed(int key);
    void    keyReleased(int key);

    ofImage brushImage;
    ofMesh  brush;
    ofFbo   maskFbo;

    float   brushOpacity;
    float   brushScale;

    int     mouseX;
    int     mouseY;
    float   hIndex;
    
    int     tx;
    int     ty;
    
    ofImage *maskFboImage;

    int     width;
    int     height;
    
private:
    void    write(string filename);
    void    read(string filename);
    
    ofPixels hPixels;
    ofPixels maskFboPixels;

    int     brushWidth;
    int     brushHeight;
    bool    mouseDown;
    bool    erase;
    int     pIndex;
    bool    bufferAllocated;
    string  filename;

};
}
