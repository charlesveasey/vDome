#include "color.h"
using namespace vd;

Color::Color() {
    resolution = 2048;
}

void Color::setup(){
    image.setUseTexture(true);
	image.allocate(resolution, resolution, OF_IMAGE_COLOR);
}

void Color::fill(int r, int g, int b){
	image.setColor(ofColor(r,g,b));
	image.update();
}

void Color::fillBlack(){
    fill(0,0,0);
}

void Color::fillWhite(){
    fill(255,255,255);
}

void Color::fillGrey(){
    fill(127,127,127);
}

void Color::close(){
    image.clear();
}

void Color::setResolution(int r){
    resolution = r;
}

void Color::bind() {
	image.getTexture().setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
	image.getTexture().bind();
}

void Color::unbind() {
	image.getTexture().unbind();
}

ofPixels & Color::getPixels(){
    return image.getPixels();
}



