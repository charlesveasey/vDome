#pragma once

#include "ofMain.h"
#include "hemisphere.h"
#include "ofxSyphon.h"
#include "ofxHapPlayer.h"

class vdome : public ofBaseApp {
	
public:
		
    void setup();
    void update();
    void draw();
    void drawFbo(int i);
    void keyPressed(int key);    
    
    ofVec3f sphToCar(ofVec3f t);

    // xml
    ofXml xml;
    void loadXML();
    
    // key map
    int keyControl;
    
    // input
    int input;
    ofImage image;
    ofVideoPlayer video;
    ofxHapPlayer hap;    
    ofxSyphonClient syphon;
    ofVideoGrabber capture;
    
    // vitual dome
    Hemisphere hemisphere;
    
    // dome texture
    ofTexture texture;
    
    // shader
    ofShader shader;

    // projector
    vector<ofCamera> cameras;
    vector<ofFbo>fbos;
    vector<ofRectangle> views;
    vector<ofPlanePrimitive> planes;

    
    // window settings
    int wX;
    int wY;
    int wWidth;
    int wHeight;

    
    // render settings
    bool vSync;
    int frameRate;
    int domeMaster;
    
    // projector settings
    int pCount;
    float pWidth;
    float pHeight;
    vector<float> pAzimuth;
    float pElevation;
    float pDistance;
    float pTilt;
    float pFov;
    float pLensOffsetX;
    float pLensOffsetY;
    vector<float> pBrightness;
    vector<float> pContrast;
    vector<float> pSaturation;
    
};
	
