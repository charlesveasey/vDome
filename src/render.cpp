#include "render.h"

void Render::init(){
    domeMaster = 2048;
    frameRate = 60;
    vSync = false;
}

void Render::setup() {
    ofSetVerticalSync(vSync);
	ofSetFrameRate(frameRate);
    
	ofEnableDepthTest();
    //ofEnableAlphaBlending();
	//ofDisableArbTex();
    ofBackground(0,0,0);
	
    //ofEnableSmoothing();
	ofEnableNormalizedTexCoords();
}

void Render::loadXML(ofXml &xml) {
    if (xml.exists("[@domeMaster]"))
        domeMaster = ofToInt( xml.getAttribute("[@domeMaster]") );
    if (xml.exists("[@frameRate]"))
        frameRate = ofToInt( xml.getAttribute("[@frameRate]") );
    if (xml.exists("[@vSync]"))
        vSync = ofToBool( xml.getAttribute("[@vSync]") );
    setup();
}

void Render::saveXML(ofXml &xml) {
    xml.setAttribute("[@domeMaster]", ofToString(domeMaster) );
    xml.setAttribute("[@frameRate]", ofToString(frameRate) );
    xml.setAttribute("[@vSync]", ofToString(vSync) );
}