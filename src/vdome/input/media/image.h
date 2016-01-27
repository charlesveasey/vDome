#pragma once
#include "ofMain.h"
namespace vd{

class Image {
    
public:
    Image();
    
    void    open(string filepath);
    void    close();
	bool    isLoaded();
    float   getWidth();
    float   getHeight();

	void	bind();
	void	unbind();
    
    ofPixels&   getPixels();

private:
	bool		bLoaded;
	ofImage		img;
};
}