#include "mask.h"

/******************************************
 
 CONSTRUCTOR
 
 ********************************************/

Mask::Mask(){

    mouseX = 0;
    mouseY = 0;
    
    width = 1024;
    height = 768;
    
    mouseDown = false;
    
    brushImage.setImageType(OF_IMAGE_COLOR_ALPHA);
    brushImage.loadImage("media/brush copy.png");
    
    maskFbo.allocate(width, height, GL_RGBA);
    renderFbo.allocate(width, height, GL_RGBA);
    
    maskFbo.begin();
        ofClear(255);
    maskFbo.end();
}

/******************************************
 
 SETUP
 
 ********************************************/

void Mask::setup(){
}

void Mask::update(){
}

void Mask::begin(){
}

void Mask::end(){
}

void Mask::draw(){
    if (mouseDown) {
        int brushImageSize = 200;
        int brushImageX = mouseX - brushImageSize * 0.5;
        int brushImageY = mouseY - brushImageSize * 0.5;
        
        maskFbo.begin();
            ofSetColor(128);
            //ofRect(brushImageX, brushImageY, brushImageSize, brushImageSize);
            brushImage.draw(brushImageX, brushImageY, brushImageSize, brushImageSize);
        maskFbo.end();
    }
}

/******************************************
 
 MOUSE
 
 ********************************************/

void Mask::onMousePressed(ofMouseEventArgs& mouseArgs){
    mouseDown = true;
    mouseX = mouseArgs.x;
    mouseY = mouseArgs.y;
}

void Mask::onMouseDragged(ofMouseEventArgs& mouseArgs){
    mouseX = mouseArgs.x;
    mouseY = mouseArgs.y;
}

void Mask::onMouseReleased(ofMouseEventArgs& mouseArgs){
    mouseDown = false;
}
