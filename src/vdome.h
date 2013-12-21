#pragma once

#include "ofMain.h"
#include "input.h"
#include "window.h"
#include "mesh.h"
#include "projector.h"
#include "tcp.h"

class vdome : public ofBaseApp {
	
public:
		
    void setup();
    void update();
    void draw();
    void drawConfig();
    void mousePressed(ofMouseEventArgs& mouseArgs);
    void mouseDragged(ofMouseEventArgs& mouseArgs);
    void mouseReleased(ofMouseEventArgs& mouseArgs);
    void keyPressed(int key);
    void keyReleased(int key);
    
    ofVec3f sphToCar(ofVec3f t);
        
    Tcp tcp;
    
    // xml
    ofXml xml;
    string xmlFile;
    void loadXML(string file);
    void saveXML(string file);
    
    // key map
    int editMode;
    int editGroup;
    int keyControl;
    
    // input
    Input input;
    
    // render
    Render render;
    
    // window
    Window window;
    
    // vitual dome
    Mesh mesh;
    
    // shader
    ofShader shader;

    // projectors
    int pCount;
    vector<Projector> projectors;

    int active;
    bool config;
    bool showConfig;
    
};
	
