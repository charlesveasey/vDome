#pragma once

#include "ofMain.h"
#include "plane.h"
#include "camera.h"

class Projector {
	
public:
    
    void init(int i);
    void setup();
    
    void mousePressed(ofMouseEventArgs& mouseArgs);
    void mouseDragged(ofMouseEventArgs& mouseArgs);
    void mouseReleased(ofMouseEventArgs& mouseArgs);
    void keyPressed(int key);
    void keyReleased(int key);
    
    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);
    
    void begin();
    void end();
    
    void bind();
    void unbind();
    
    void draw();
        
    ofVec3f sphToCar(ofVec3f t);
    
    camera camera;
    ofFbo fbo;
    ofRectangle view;
    
    int index;
    
    bool keyboard;
    bool mouse;

    string xmlPrefix;
    
    float width;
    float height;
    
    float azimuth;
    float elevation;
    float distance;
    
    float pan;
    float tilt;
    float roll;
    
    float fov;
    
    float lensOffsetX;
    float lensOffsetY;
   
    vector<float> shear;
   
    Plane plane;
    
    vector<float> scale;
    
    float brightness;
    float contrast;
    float saturation;
    
    int editMode;
    bool superKey;
    float value;
    
};

