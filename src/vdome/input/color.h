#pragma once
#include "ofMain.h"
namespace vd{

class Color {
    
public:
    Color();
    void bind();
    void unbind();
    void fill(int r, int g, int b);
    void fillBlack();
    void fillWhite();
    void fillGrey();
    void close();
    void setResolution(int r);
    
private:
    ofImage image;
    int resolution;
    ofColor c;
};
}