#include "Image.h"
using namespace vd;

Image::Image() {
    bUseTimer = false;
    bTimerRunning = false;
	bLoaded = false;
    timerDuration = 10;
}

void Image::open(string filepath, int w, int h){
    img.clear();
    img.allocate(w, h, OF_IMAGE_COLOR);
    bLoaded = img.loadImage(filepath);
    if (bUseTimer && timerDuration > 0)
        startTimer();
}

void Image::update(){
    if(bTimerRunning)
        evalTimer();
}

void Image::bind(){
    img.getTextureReference().setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
    img.getTextureReference().bind();
}

void Image::unbind(){
    img.getTextureReference().unbind();
}

void Image::play(){
    startTimer();
}

void Image::stop(){
    stopTimer();
}

void Image::close(){
    img.clear();
	bLoaded = false;
}

// timer
void Image::setUseTimer(bool uTimer){
    bUseTimer = uTimer;
}

void Image::setTimerDuration(float dur){
    timerDuration = dur;
}

void Image::startTimer(){
    sTime = ofGetElapsedTimeMillis();
    eTime = 1000 * timerDuration;
    bTimerRunning = true;
}

void Image::evalTimer(){
    float t = ofGetElapsedTimeMillis() - sTime;
    if (t >= eTime) {
        stopTimer();
        ofNotifyEvent(timerEvent,bTimerRunning,this);
    }
}

void Image::stopTimer(){
    sTime = 0;
    bTimerRunning = false;
}

bool Image::isLoaded(){
	return bLoaded;
}