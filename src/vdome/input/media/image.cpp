#include "image.h"
using namespace vd;

Image::Image() {
	bLoaded = false;
}

void Image::open(string filepath){
    img.clear();
    bLoaded = img.load(filepath);
}

void Image::close(){
    img.clear();
	bLoaded = false;
}

bool Image::isLoaded(){
	return bLoaded;
}

float Image::getWidth(){
	return img.getWidth();
}

float Image::getHeight(){
	return img.getHeight();
}

ofPixels& Image::getPixels(){
    return img.getPixels();
}

void Image::bind(){
	img.getTexture().setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
	img.getTexture().bind();
}

void Image::unbind() {
	img.getTexture().unbind();
}