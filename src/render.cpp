#include "render.h"
namespace vd {

/******************************************

 CONSTRUCTOR

 ********************************************/

Render::Render(){
    framerate = 60;
    vsync = true;
}

/******************************************

 SETUP

 ********************************************/

void Render::setup() {
    ofSetVerticalSync(vsync);
	ofSetFrameRate(framerate);

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
    return vsync;

}
void Render::setVSync(bool val) {
    vsync = val;
    ofSetVerticalSync(vsync);
}

int Render::getFrameRate() {
    return framerate;
}
void Render::setFrameRate(int val) {
    framerate = val;
    ofSetFrameRate(framerate);
}

/******************************************

 SETTINGS

 ********************************************/

void Render::loadXML(ofXml &xml) {
    if (xml.exists("[@vsync]")) {
        string str = ofToString( xml.getAttribute("[@vsync]") );
        if (str == "on")    vsync = true;
        else                vsync = false;
    }
    if (xml.exists("[@framerate]"))
        framerate = ofToInt( xml.getAttribute("[@framerate]") );
    setup();
}

void Render::saveXML(ofXml &xml) {
    if (vsync)  xml.setAttribute("vsync", "on" );
    else        xml.setAttribute("vsync", "off" );

    xml.setAttribute("framerate", ofToString(framerate) );
}

}
