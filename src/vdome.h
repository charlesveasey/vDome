#pragma once

#include "ofMain.h"
#include "input.h"
#include "window.h"
#include "dome.h"
#include "projector.h"
#include "tcp.h"

class vdome : public ofBaseApp {
	
public:
    // tcp
    Tcp tcp;
    
    // input
    Input input;
    
    // render
    Render render;
    
    // window
    Window window;
    
    // dome mesh
    Dome dome;
    
    // shader
    ofShader shader;
    
    // projector
    
    vector<Projector> projectors;
    
    // projector count
    int pCount;
    
    // active projector index
    int pActive;
    
    
    // config
    bool config;
    bool showConfig;
    bool showFrameRate;
    
    
	// cycle
    void setup();
    void update();
    void draw();
    void drawConfig();
    
    // mouse
    void mousePressed(ofMouseEventArgs& mouseArgs);
    void mouseDragged(ofMouseEventArgs& mouseArgs);
    void mouseReleased(ofMouseEventArgs& mouseArgs);
    
    // keyboard
    void keyPressed(int key);
    void keyReleased(int key);
    
    // math
    ofVec3f sphToCar(ofVec3f t);
    float roundTo(float val, float n);
    
    // xml
    ofXml xml;
    string xmlFile;
    void loadXML(string file);
    void saveXML(string file);
    
    // config
    int editMode;
    int editGroup;    
    
    // keyboard
    bool shift;
    bool alt;
    bool ctrl;
    bool mod;
};
	
