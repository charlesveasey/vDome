#pragma once
#include "ofVideoGrabber.h"
namespace vd{
class Capture {
    
public:
    Capture();
    void    open();
    void    update();
    void    play();
    void    stop();
    void    close();
    bool    isOpen();
    void    setResolution(int r);
    void    setFramerate(int frate);
    float   getRealWidth();
    float   getRealHeight();
    
    ofPixels & getPixels();
    
    void    bind();
    void    unbind();
    
private:
    ofVideoGrabber  grabber;
    int             resolution;
};
}