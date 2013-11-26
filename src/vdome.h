#pragma once

#include "ofMain.h"
#include "window.h"
#include "hemisphere.h"
#include "projector.h"
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
    
    
    // window
    Window window;
    
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

    // projectors
    int pCount;
    vector<Projector> projectors;
    
    // render settings
    int frameRate;
    int domeMaster;
    
};
	
