#pragma once
#include "ofMain.h"
namespace vd{

class Color {
    
public:
    Color();
	void setup();
    void bind();
    void unbind();
    void fill(int r, int g, int b);
    void fillBlack();
    void fillWhite();
    void fillGrey();
    void close();
    
private:
    ofImage image;
    ofColor c;
};
}