#pragma once
#include "ofMain.h"
namespace vd{

class Color {
    
public:
    Color();
	void    setup();
    void    fill(int r, int g, int b);
    void    fillBlack();
    void    fillWhite();
    void    fillGrey();
    void    close();
    void    setResolution(int r);
	void	bind();
	void	unbind();
    
    ofPixels&   getPixels();
    
private:
    ofImage image;
    int     resolution;
};
}