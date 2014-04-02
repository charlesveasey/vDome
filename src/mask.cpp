#include "mask.h"

/******************************************
 
 CONSTRUCTOR
 
 ********************************************/

Mask::Mask(){
    
    mouseX = 0;
    mouseY = 0;
    
    width = 1024;
    height = 768;
    
    brushImageSize = 256;
    brushImageAlpha = 10;
    
    mouseDown = false;
    
    brushImage.setUseTexture(true);
    brushImage.allocate(brushImageSize, brushImageSize, OF_IMAGE_COLOR_ALPHA);
    brushImage.setImageType(OF_IMAGE_COLOR_ALPHA);
    brushImage.loadImage("brushes/brush.png");
        
    brush = ofMesh::plane(brushImageSize, brushImageSize, 2, 2, OF_PRIMITIVE_TRIANGLES);
    
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
    
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    maskFbo.begin();

    if (mouseDown) {
        brushImage.bind();
        ofSetColor(255, 255, 255, brushImageAlpha);
        ofPushMatrix();
            ofTranslate(mouseX, mouseY);
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
