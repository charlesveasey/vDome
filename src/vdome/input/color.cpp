#include "color.h"
using namespace vd;

Color::Color() {
    resolution = 2048;
}

void Color::bind(){
    image.getTextureReference().bind();
}

void Color::unbind(){
    image.getTextureReference().unbind();
}

void Color::fill(int r, int g, int b){
    c.set(r, g, b);
    close();
    image.allocate(resolution, resolution, OF_IMAGE_COLOR);
    for (int x=0; x < resolution; x++) {
        for (int y=0; y < resolution; y++)
            image.setColor(x, y, c);
    }
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

void Color::setResolution(int r){
    resolution = r;
}

void Color::close(){
    image.clear();
}




