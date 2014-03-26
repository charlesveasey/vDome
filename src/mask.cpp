#include "mask.h"

/******************************************
 
 CONSTRUCTOR
 
 ********************************************/

ofMesh brush;

Mask::Mask(){
    


    mouseX = 0;
    mouseY = 0;
    
    width = 1024;
    height = 768;
    
    mouseDown = false;
    
    brushImage.setUseTexture(true);
    brushImage.allocate(256, 256, OF_IMAGE_COLOR_ALPHA);
    brushImage.setImageType(OF_IMAGE_COLOR_ALPHA);
    brushImage.loadImage("media/brush copy.png");
    
    
    brush = ofMesh::plane(256, 256, 2, 2, OF_PRIMITIVE_TRIANGLES);

    
    maskFbo.allocate(width, height, GL_RGBA);
    
    maskFbo.begin();
        ofClear(255,255,255,0);
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
    ofEnableAlphaBlending();
    
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    maskFbo.begin();
    //ofClear(255,255,255,0);

    if (mouseDown) {



        int brushImageSize = 200;
        int brushImageX = mouseX - 256 * 0.5;
        int brushImageY = mouseY - 256 * 0.5;
        

        brushImage.bind();
        
        ofPushMatrix();
        ofTranslate(brushImageX, brushImageY);
        
        ofSetColor(255, 255, 255, 10);
        
        brush.draw();
        ofPopMatrix();
        brushImage.unbind();
        
        //ofSetColor(255, 255, 255, 255);

        
        //brushImage.bind();
    
      //  ofRect(brushImageX, brushImageY, brushImageSize, brushImageSize);
       //// ofCircle(brushImageX, brushImageY, brushImageSize/2);
        
        //brushImage.unbind();


       // ofDisableAlphaBlending();
    }
    
    maskFbo.end();
    
    ofDisableBlendMode();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
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
