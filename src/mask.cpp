#include "mask.h"
namespace vd {

extern float projWidth;
extern float projHeight;
extern int maxHistory;

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
    maskFboImage.bpp = 16;
    brushImage.loadImage("brushes/brush.png");
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
    width = projWidth;
    height = projHeight;

    if (!bufferAllocated) {
        for (int i=0; i<=(maxHistory+2); i++) {
            ofPixels buffer;
            buffer.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
            history.push_back(buffer);
        }
    }

    bufferAllocated = true;

    if (maskFbo.getWidth() != projWidth || maskFbo.getHeight() != projHeight)
        maskFbo.allocate(width, height, GL_RGBA32F_ARB);

    maskFbo.begin();
        ofClear(255,255,255,0);
    maskFbo.end();

    if (maskFboImage.isAllocated()) {
        ofDisableAlphaBlending();
        ofDisableNormalizedTexCoords();
        maskFbo.begin();
        ofSetColor(255, 255, 255, 255);
            maskFboImage.draw(0,0, 1024, 768);
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
            ofEnableBlendMode(OF_BLENDMODE_ADD);

        maskFbo.begin();
        brushImage.bind();
        ofSetColor(brushOpacity, brushOpacity, brushOpacity, brushOpacity);
        ofPushMatrix();
            ofTranslate(mouseX-(width*pIndex), mouseY);
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

void Mask::load(){
    string filename;
    filename = "masks/mask-" + ofToString(pIndex+1) + ".png";
    read(filename);
}

void Mask::save(){
    string filename;
    filename = "masks/mask-" + ofToString(pIndex+1) + ".png";
    write(filename);
}

void Mask::write(string filename){
    maskFboPixels.clear();
    maskFbo.readToPixels(maskFboPixels);
    maskFboImage.setFromPixels(maskFboPixels);
    maskFboImage.saveImage(filename);
}

void Mask::read(string filename){
    maskFboImage.clear();
    if (maskFboImage.loadImage(filename))
        setup();
}

int Mask::store(int fIndex) {
    hIndex = fIndex;
    hPixels.clear();
    maskFbo.readToPixels(hPixels);
    hPixels.pasteInto(history[fIndex], 0, 0);
    return fIndex;
}

void Mask::recall(int fIndex) {
    maskFboImage.clear();
    maskFboImage.setFromPixels(history[fIndex]);
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
