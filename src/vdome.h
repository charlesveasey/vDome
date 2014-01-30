#pragma once

#include "ofMain.h"
#include "input.h"
#include "window.h"
#include "dome.h"
#include "projector.h"
#include "tcp.h"

#ifdef TARGET_OSX
    #include "../../../libs/glut/lib/osx/GLUT.framework/Versions/A/Headers/glut.h"
#endif
#ifdef TARGET_WIN32
    #include <windows.h>
#endif

class vdome : public ofBaseApp {
	
public:
    vdome();
    
private:
    Tcp tcp;
    Input input;
    Render render;
    Window window;
    Dome dome;
    ofShader shader;
    
    int pCount;
    int pActive;
    vector<Projector> projectors;
    
    bool config;
    bool showConfig;
    bool showFrameRate;
    
    void setup();
    void update();
    void draw();
    void drawConfig();
    int editMode;
    int editGroup;
    int frameCnt;
    bool saved;
    
    bool shift;
    bool alt;
    bool ctrl;
    bool mod;
    
    float value;
    float orgValue;
    float shiftValue;
    float altValue;
    
    void keyPressed(int key);
    void keyReleased(int key);
    
    void mousePressed(ofMouseEventArgs& mouseArgs);
    void mouseDragged(ofMouseEventArgs& mouseArgs);
    void mouseReleased(ofMouseEventArgs& mouseArgs);
    
    ofVec3f sphToCar(ofVec3f t);
    float roundTo(float val, float n);
    
    ofXml xml;
    string xmlFile;
    void loadXML(string file);
    void saveXML(string file);
};
	
