#include "mask.h"

/******************************************
 
 CONSTRUCTOR
 
 ********************************************/

Mask::Mask(){
    erase = false;
    
    mouseX = 0;
    mouseY = 0;
    
    width = 1024;
    height = 768;
    
    mouseDown = false;
    
    brushImage.setUseTexture(true);
    brushImage.setImageType(OF_IMAGE_COLOR_ALPHA);
    brushImage.loadImage("brushes/brush.png");
    brushWidth = brushImage.getWidth();
    brushHeight = brushImage.getHeight();
    brushScale = 1;
    brushOpacity = 50;
    
    brush = ofMesh::plane(brushWidth, brushHeight, 2, 2, OF_PRIMITIVE_TRIANGLES);
}

/******************************************
 
 SETUP
 
 ********************************************/

void Mask::setup(){    
    maskFbo.allocate(width, height);
    
    maskFbo.begin();
        ofClear(255,255,255,0);
    maskFbo.end();
    
    ofDisableNormalizedTexCoords();
    maskFbo.begin();
    ofSetColor(255, 255, 255, 255);
        maskFboImage.draw(0,0, 1024, 768);
    maskFbo.end();
    ofEnableNormalizedTexCoords();
}

/******************************************
 
 DRAW
 
 ********************************************/

void Mask::draw(){
    if (mouseDown) {
        if (erase)
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        else
            ofEnableBlendMode(OF_BLENDMODE_SCREEN);

        maskFbo.begin();
        brushImage.bind();
        ofSetColor(brushOpacity, brushOpacity, brushOpacity, brushOpacity);
        ofPushMatrix();
            ofTranslate(mouseX-(width*indx), mouseY);
            ofScale(brushScale, brushScale);
            brush.draw();
        ofPopMatrix();
        brushImage.unbind();
        maskFbo.end();
        
        ofDisableBlendMode();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    }    
}

/******************************************
 
 MOUSE
 
 ********************************************/

void Mask::mousePressed(ofMouseEventArgs& mouseArgs){
    mouseDown = true;
    mouseX = mouseArgs.x;
    mouseY = mouseArgs.y;
}

void Mask::mouseDragged(ofMouseEventArgs& mouseArgs){
    mouseX = mouseArgs.x;
    mouseY = mouseArgs.y;
}

void Mask::mouseReleased(ofMouseEventArgs& mouseArgs){
    mouseDown = false;
}

/******************************************
 
 KEYBOARD
 
 ********************************************/

void Mask::keyPressed(int key){
    if (key == OF_KEY_CONTROL || key == OF_KEY_COMMAND) {
        erase = true;
    }
}

void Mask::keyReleased(int key){
    if (key == OF_KEY_CONTROL || key == OF_KEY_COMMAND) {
        erase = false;
    }
}

/******************************************
 
 SETTINGS
 
 ********************************************/

void Mask::load(int i){
    indx = i;
    string filename;
    filename = "masks/mask-" + ofToString(i) + ".png";
    maskFboImage.loadImage(filename);
    setup();
}

void Mask::save(int i){
    ofPixels maskFboPixels;
    maskFbo.readToPixels(maskFboPixels);
    maskFboImage.setFromPixels(maskFboPixels);
    string filename;
    filename = "masks/mask-" + ofToString(i) + ".png";    
    maskFboImage.saveImage(filename);
}

/******************************************
 
 RESET
 
 ********************************************/

void Mask::reset(){
    maskFbo.begin();
        ofClear(255,255,255,0);
    maskFbo.end();
}
