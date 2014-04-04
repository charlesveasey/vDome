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
    
    brushOpacity = 10;
    
    mouseDown = false;
    
    brushImage.setUseTexture(true);
    brushImage.setImageType(OF_IMAGE_COLOR_ALPHA);
    brushImage.loadImage("brushes/brush.png");
    brushWidth = brushImage.getWidth();
    brushHeight = brushImage.getHeight();
    
    brushScale = 1;
    
    brush = ofMesh::plane(brushWidth, brushHeight, 2, 2, OF_PRIMITIVE_TRIANGLES);
    
    maskFbo.allocate(width, height, GL_RGBA);
    
    maskFbo.begin();
        ofClear(255,255,255,0);
    maskFbo.end();
}

/******************************************
 
 DRAW
 
 ********************************************/

void Mask::draw(){
    ofEnableAlphaBlending();
    
    if (erase)
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    else
        ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    
    maskFbo.begin();

    if (mouseDown) {
        brushImage.bind();
        ofSetColor(255, 255, 255, brushOpacity);
        ofPushMatrix();
            ofTranslate(mouseX, mouseY);
            ofScale(brushScale, brushScale);
            brush.draw();
        ofPopMatrix();
        brushImage.unbind();
    }
    
    maskFbo.end();
    
    ofDisableBlendMode();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
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
