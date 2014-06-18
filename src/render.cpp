#include "render.h"
namespace vd {

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

    ofBackground(0,0,0);
	//ofEnableDepthTest();
    ofEnableNormalizedTexCoords();

    //ofEnableAntiAliasing();
    //ofEnableSmoothing();
    ofEnableAlphaBlending();
	//ofDisableArbTex();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

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
    if (xml.exists("[@vSync]")) {
        string str = ofToString( xml.getAttribute("[@vSync]") );
        if (str == "on")    vSync = true;
        else                vSync = false;
    }
    if (xml.exists("[@frameRate]"))
        frameRate = ofToInt( xml.getAttribute("[@frameRate]") );
    setup();
}

void Render::saveXML(ofXml &xml) {
    if (vSync)  xml.setAttribute("vSync", "on" );
    else        xml.setAttribute("vSync", "off" );

    xml.setAttribute("frameRate", ofToString(frameRate) );
}

}
