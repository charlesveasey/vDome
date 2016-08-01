#include "capture.h"
using namespace vd;

Capture::Capture() {
    resolution = 2048;
}

void Capture::open(){
    grabber.setUseTexture(true);
    grabber.setup(resolution, resolution);
}

void Capture::update(){
    if (isOpen()){
        grabber.update();
    }
}

void Capture::close(){
    if (isOpen()){
        grabber.close();
    }
}

bool Capture::isOpen(){
    return grabber.isInitialized();
}

void Capture::setResolution(int r){
    resolution = r;
}

void Capture::setFramerate(int frate){
    close();
    grabber.setDesiredFrameRate(frate);
    close();
}

float Capture::getRealWidth(){
	return grabber.getWidth();
}

float Capture::getRealHeight(){
	return grabber.getHeight();
}

ofPixels& Capture::getPixels(){
    return grabber.getPixels();
}

void Capture::bind(){
	grabber.getTexture().setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
    grabber.getTexture().bind();
}

void Capture::unbind(){
    grabber.getTexture().unbind();
}

