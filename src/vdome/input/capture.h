#pragma once
#include "ofPBO.h"
#include "ofVideoGrabber.h"
namespace vd{

class Capture {
    
public:
    Capture();
    
    void open();
    void update();
    void bind();
    void unbind();
    void play();
    void stop();
    void close();
    bool isOpen();
    void setResolution(int r);

    ofVideoGrabber grabber;
    ofPixels pixels;
    ofPBO pbo;

private:
    bool bPbo;
    int resolution;
};
}