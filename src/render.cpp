#include "render.h"

/******************************************
 
 CONSTRUCTOR
 
 ********************************************/

Render::Render(){
    frameRate = 60;
    vSync = true;
}

/******************************************
 
 SETUP
 
 ********************************************/

void Render::setup() {
    ofSetVerticalSync(vSync);
	ofSetFrameRate(frameRate);
    
    ofBackground(100,0,0);
	ofEnableDepthTest();
    ofEnableNormalizedTexCoords();
    
    //ofEnableSmoothing();
    //ofEnableAlphaBlending();
	//ofDisableArbTex();
}

/******************************************
 
 ACCESSORS
 
 ********************************************/

bool Render::getVSync() {
    return vSync;
  	
}
void Render::setVSync(bool val) {
    vSync = val;
    ofSetVerticalSync(vSync);
}

int Render::getFrameRate() {
    return frameRate;  	
}
void Render::setFrameRate(int val) {
    frameRate = val;
    ofSetFrameRate(frameRate);
}

/******************************************
 
 SETTINGS
 
 ********************************************/

void Render::loadXML(ofXml &xml) {
    if (xml.exists("[@vSync]"))
        vSync = ofToBool( xml.getAttribute("[@vSync]") );
    if (xml.exists("[@frameRate]"))
        frameRate = ofToInt( xml.getAttribute("[@frameRate]") );
    setup();
}

void Render::saveXML(ofXml &xml) {
    xml.setAttribute("[@vSync]", ofToString(vSync) );
    xml.setAttribute("[@frameRate]", ofToString(frameRate) );    
}