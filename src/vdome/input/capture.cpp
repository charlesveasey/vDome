#include "capture.h"
using namespace vd;

Capture::Capture() {
    bPbo = false;
    resolution = 2048;
}

void Capture::open(){
    grabber.initGrabber(resolution, resolution);
    if (bPbo) {
        grabber.setUseTexture(false);
        pbo.allocate(2, resolution, resolution);
    }
}

void Capture::bind(){
    if (bPbo) {
        pbo.texture.setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
        pbo.texture.bind();
    }
    else {
        grabber.getTextureReference().setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
        grabber.getTextureReference().bind();
    }
}

void Capture::unbind(){
    if (bPbo)
        pbo.texture.unbind();
    else
        grabber.getTextureReference().unbind();
}

void Capture::update(){
    if (isOpen()){
        grabber.update();
        if (bPbo) {
            pixels = grabber.getPixelsRef();
            if (pixels.isAllocated())
                pbo.loadData(pixels);
        }
    }
}

void Capture::close(){
    if (isOpen()){
        if (bPbo)
            pbo.wait();
        grabber.close();
    }
}

bool Capture::isOpen(){
    return grabber.isInitialized();
}

void Capture::setResolution(int r){
    resolution = r;
}

float Capture::getRealWidth(){
	return grabber.getWidth();
}

float Capture::getRealHeight(){
	return grabber.height;
}