#include "color.h"
using namespace vd;

Color::Color() {
}

void Color::setup(){
	image.allocate(4, 4, OF_IMAGE_COLOR);
}

void Color::bind(){
    image.getTextureReference().bind();
}

void Color::unbind(){
    image.getTextureReference().unbind();
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




