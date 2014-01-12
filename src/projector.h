#pragma once

#include "ofMain.h"
#include "plane.h"
#include "camera.h"
#include "command.h"

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
    
    vector<float> shear;
    vector<float> scale;
    vector<float> offset;
    
    Plane plane;
    CommandHistory history;
    
    float brightness;
    float contrast;
    float saturation;
    
    int editMode;
    bool superKey;
    float value;
    
};

class SetBrightness : public Command {
protected:
    Projector& obj;
    float value;
public:
    SetBrightness(Projector& obj, float value) : obj(obj), value(value) {}
    void execute() {
        obj.brightness = value;
    }
    void undo() {
        obj.brightness = value;
    }
};