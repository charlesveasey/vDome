#include "mask.h"
namespace vd {
    
extern int maxHistory;
extern vector<ofPixels> maskHistory;

/******************************************

 CONSTRUCTOR

 ********************************************/

Mask::Mask(){
    erase = false;

    mouseX = 0;
    mouseY = 0;

    tx = 0;
    ty = 0;

    mouseDown = false;
    maskFboImage = new ofImage();
    brushImage.setUseTexture(true);
    brushImage.setImageType(OF_IMAGE_COLOR_ALPHA);
    brushImage.load("settings/brushes/brush.png");
    brushWidth = brushImage.getWidth();
    brushHeight = brushImage.getHeight();
    brushScale = 1;
    brushOpacity = 50;

    brush = ofMesh::plane(brushWidth, brushHeight, 2, 2, OF_PRIMITIVE_TRIANGLES);

    hIndex = 0;
	bufferAllocated = false;
}

/******************************************

 INIT

 ********************************************/

void Mask::init(int i){
    pIndex = i;
}

/******************************************

 SETUP

 ********************************************/

void Mask::setup(){

    if (maskFbo.getWidth() != width || maskFbo.getHeight() != height)
        maskFbo.allocate(width, height, GL_RGBA32F_ARB);

    maskFbo.begin();
        ofClear(255,255,255,0);
    maskFbo.end();

    if (maskFboImage->isAllocated()) {
        ofDisableAlphaBlending();
        ofDisableNormalizedTexCoords();
        maskFbo.begin();
        ofSetColor(255, 255, 255, 255);
            maskFboImage->draw(0,0, width, height);
        maskFbo.end();
        ofEnableNormalizedTexCoords();
        ofEnableAlphaBlending();
    }
}

/******************************************

 DRAW

 ********************************************/

void Mask::draw(){
    if (mouseDown) {
        
        if (erase)
            ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
        else
            ofEnableBlendMode(OF_BLENDMODE_SCREEN);

        maskFbo.begin();
        brushImage.bind();
        ofSetColor(brushOpacity, brushOpacity, brushOpacity, brushOpacity);
        ofPushMatrix();
            ofTranslate(mouseX-tx, mouseY);
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
    if (hIndex >= (maxHistory+2) )
        hIndex = 0;
    store(hIndex);
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

void Mask::load(int projectorStartingIndex){
    string filename;
    filename = "settings/masks/mask-" + ofToString(pIndex+1+projectorStartingIndex) + ".png";
    read(filename);
}

void Mask::save(int projectorStartingIndex){
    string filename;
    filename = "settings/masks/mask-" + ofToString(pIndex+1+projectorStartingIndex) + ".png";
    write(filename);
}

void Mask::write(string filename){
    maskFboPixels.clear();
    maskFbo.readToPixels(maskFboPixels);
    maskFboImage->setFromPixels(maskFboPixels);
}

void Mask::read(string filename){
    maskFboImage->clear();
    if (maskFboImage->load(filename))
        setup();
}

int Mask::store(int fIndex) {
    hIndex = fIndex;
    hPixels.clear();
    maskFbo.readToPixels(hPixels);
    hPixels.pasteInto(maskHistory[fIndex], 0, 0);
    return fIndex;
}

void Mask::recall(int fIndex) {
    maskFboImage->clear();
    maskFboImage->setFromPixels(maskHistory[fIndex]);
    setup();
}

/******************************************

 RESET

 ********************************************/

void Mask::reset(){
    maskFbo.begin();
        ofClear(255,255,255,0);
    maskFbo.end();
}

}
