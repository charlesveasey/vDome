#pragma once

#include "ofMain.h"
#include "plane.h"
#include "camera.h"
#include "command.h"

class Projector {
   
public:
    int index;
    bool keyboard;
    bool mouse;
    CommandHistory history;
    int editMode;
    bool mod;
    bool all;    
    void setValue(float v);
    
    // intensity
    float brightness;
    float contrast;
    
    // color
    float saturation;
    
    // plane
    ofVec2f getPlanePosition();
    void setPlanePosition(float x, float y);
    
    ofVec2f getPlaneDimensions();
    void setPlaneDimensions(float w, float h);
    
    // keystone flag
    bool getKeystoneActive();
    void setKeystoneActive(bool v);
    
    // grid flag
    bool getGridActive();
    void setGridActive(bool v);
    
    
    // camera
    void  setCameraTransform();
    
    ofVec3f getCameraPosition();
    void setCameraPosition(float azi, float ele, float dis);

    ofVec3f getCameraOrientation();
    void setCameraOrientation(float roll, float tilt, float pan);
    
    float getCameraFov();
    void setCameraFov(float v);

    ofVec2f getCameraOffset();
    void setCameraOffset(float x, float y);
    
    ofVec2f getCameraScale();
    void setCameraScale(float x, float y);
    
    vector<float> getCameraShear();
    void setCameraShear(vector<float>);
    
    
    // cycle
    void init(int i);
    void setup();
    void begin();
    void end();
    void bind();
    void unbind();
    void draw();
    
    void drawPlaneConfig();
    void drawKeystone();
    
    ofTexture& getTextureReference();
    
    // mouse
    void mousePressed(ofMouseEventArgs& mouseArgs);
    void mouseDragged(ofMouseEventArgs& mouseArgs);
    void mouseReleased(ofMouseEventArgs& mouseArgs);
    
    
    // keyboard
    void keyPressed(int key);
    void keyReleased(int key);
    
    // xml
    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);
    
private:
    camera camera;
    ofFbo fbo;
    ofRectangle view;
    Plane plane;

    float value;
    
    string xmlPrefix;
    ofVec3f sphToCar(ofVec3f t);

    // plane
    ofVec2f planePosition;
    ofVec2f planeDimensions;
    
    // camera
    ofVec3f cameraPosition;
    ofVec3f cameraOrientation;
    float cameraFov;
    ofVec2f cameraOffset;
    ofVec2f cameraScale;
    vector<float> cameraShear;
};



/******************************************
 
 UNDO/REDO COMMANDS
 
 ********************************************/


// intensity
class SetBrightness : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetBrightness(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() { obj.brightness = v; }
    void undo() { obj.brightness = v; }
};

class SetContrast : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetContrast(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() { obj.contrast = v; }
    void undo() { obj.contrast = v; }
};



// color
class SetSaturation : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetSaturation(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() { obj.saturation = v; }
    void undo() { obj.saturation = v; }
};



// plane
class SetPlanePosition : public Command {
protected:
    Projector& obj;
    float x;
    float y;
public:
    SetPlanePosition(Projector& obj, float x, float y) : obj(obj), x(x), y(y) {}
    void execute() { obj.setPlanePosition(x, y); }
    void undo() { obj.setPlanePosition(x, y); }
};



// camera
class SetCameraPosition : public Command {
protected:
    Projector& obj;
    float azi, ele, dis;
public:
    SetCameraPosition(Projector& obj, float azi, float ele, float dis) : obj(obj), azi(azi), ele(ele), dis(dis) {}
    void execute() { obj.setCameraPosition(azi, ele, dis); }
    void undo() { obj.setCameraPosition(azi, ele, dis); }
};

class SetCameraOrientation : public Command {
protected:
    Projector& obj;
    float roll, tilt, pan;
public:
    SetCameraOrientation(Projector& obj, float roll, float tilt, float pan) : obj(obj), roll(roll), tilt(tilt), pan(pan) {}
    void execute() { obj.setCameraOrientation(roll, tilt, pan); }
    void undo() { obj.setCameraOrientation(roll, tilt, pan); }
};

class SetCameraFov : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetCameraFov(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() { obj.setCameraFov(v); }
    void undo() { obj.setCameraFov(v); }
};

class SetCameraOffset : public Command {
protected:
    Projector& obj;
    float x, y;
public:
    SetCameraOffset(Projector& obj, float x, float y) : obj(obj), x(x), y(y) {}
    void execute() { obj.setCameraOffset(x,y); }
    void undo() { obj.setCameraOffset(x,y); }
};

class SetCameraScale : public Command {
protected:
    Projector& obj;
    float x, y;
public:
    SetCameraScale(Projector& obj, float x, float y) : obj(obj), x(x), y(y) {}
    void execute() { obj.setCameraScale(x, y); }
    void undo() { obj.setCameraScale(x, y); }
};

class SetCameraShear : public Command {
protected:
    Projector& obj;
    vector<float> v;
public:
    SetCameraShear(Projector& obj, vector<float>  v) : obj(obj), v(v) {}
    void execute() { obj.setCameraShear(v); }
    void undo() { obj.setCameraShear(v); }
};
