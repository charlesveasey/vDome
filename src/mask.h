#pragma once
#include "ofMain.h"
namespace vd {

class Mask {
public:
    Mask();

    void init(int i);
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

    void save();
    void load();
    int store(int fIndex);
    void recall(int fIndex);

    void reset();

    int mouseX;
    int mouseY;
    float hIndex;

    vector<ofPixels> history;

private:
    bool mouseDown;

    int width;
    int height;

    int brushWidth;
    int brushHeight;

    bool erase;

    int pIndex;

    ofImage maskFboImage;
    string filename;
    ofPixels maskFboPixels;
    void write(string filename);
    void read(string filename);

    ofPixels hPixels;

    bool bufferAllocated;
};
}
