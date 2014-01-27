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
    
    // keystone
    bool getKeystoneActive();
    void setKeystoneActive(bool v);
    
    vector<ofPoint> getKeystonePoints();
    void setKeystonePoints(vector<ofPoint> pts);
    
    // grid
    bool getGridActive();
    void setGridActive(bool v);
    
    vector<ofVec3f> getGridPoints();
    void setGridPoints(vector<ofVec3f> v);
    
    
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
    void loadxml(ofXml &xml);
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
    float l;
public:
    SetBrightness(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        l = obj.brightness;
        obj.brightness = v; }
    void undo() { obj.brightness = l; }
};

class SetContrast : public Command {
protected:
    Projector& obj;
    float v;
    float l;
public:
    SetContrast(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        l = obj.contrast;
        obj.contrast = v; }
    void undo() { obj.contrast = l; }
};



// color
class SetSaturation : public Command {
protected:
    Projector& obj;
    float v;
    float l;
public:
    SetSaturation(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        l = obj.saturation;
        obj.saturation = v; }
    void undo() { obj.saturation = l; }
};



// plane
class SetPlanePosition : public Command {
protected:
    Projector& obj;
    float x, y;
    ofVec2f l;
public:
    SetPlanePosition(Projector& obj, float x, float y) : obj(obj), x(x), y(y) {}
    void execute() {
        l = obj.getPlanePosition();
        obj.setPlanePosition(x, y); }
    void undo() { obj.setPlanePosition(x, y); }
};

class SetKeystonePoints : public Command {
protected:
    Projector& obj;
    vector<ofPoint> v;
    vector<ofPoint> l;
public:
    SetKeystonePoints(Projector& obj, vector<ofPoint> v, vector<ofPoint> l) : obj(obj), v(v), l(l) {}
    void execute() {;
        obj.setKeystonePoints(v);
    }
    void undo() { obj.setKeystonePoints(l); }
};

class SetGridPoints : public Command {
protected:
    Projector& obj;
    vector<ofPoint> v;
    vector<ofPoint> l;
public:
    SetGridPoints(Projector& obj, vector<ofPoint> v, vector<ofPoint> l) : obj(obj), v(v), l(l) {}
    void execute() {;
        obj.setGridPoints(v);
    }
    void undo() { obj.setGridPoints(l); }
};


// camera
class SetCameraPosition : public Command {
protected:
    Projector& obj;
    float azi, ele, dis;
    ofVec3f l;
public:
    SetCameraPosition(Projector& obj, float azi, float ele, float dis) : obj(obj), azi(azi), ele(ele), dis(dis) {}
    void execute() {
        l = obj.getCameraPosition();
        obj.setCameraPosition(azi, ele, dis);
    }
    void undo() { obj.setCameraPosition(l.x, l.y, l.z); }
};

class SetCameraOrientation : public Command {
protected:
    Projector& obj;
    float roll, tilt, pan;
    ofVec3f l;
public:
    SetCameraOrientation(Projector& obj, float roll, float tilt, float pan) : obj(obj), roll(roll), tilt(tilt), pan(pan) {}
    void execute() {
        l = obj.getCameraOrientation();
        obj.setCameraOrientation(roll, tilt, pan);
    }
    void undo() { obj.setCameraOrientation(l.x, l.y, l.z); }
};

class SetCameraFov : public Command {
protected:
    Projector& obj;
    float v;
    float l;
public:
    SetCameraFov(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        l = obj.getCameraFov();
        obj.setCameraFov(v);
    }
    void undo() { obj.setCameraFov(l); }
};

class SetCameraOffset : public Command {
protected:
    Projector& obj;
    float x, y;
    ofVec2f l;
public:
    SetCameraOffset(Projector& obj, float x, float y) : obj(obj), x(x), y(y) {}
    void execute() {
        l = obj.getCameraOffset();
        obj.setCameraOffset(x,y);
    }
    void undo() { obj.setCameraOffset(l.x,l.y); }
};

class SetCameraScale : public Command {
protected:
    Projector& obj;
    float x, y;
    ofVec2f l;
public:
    SetCameraScale(Projector& obj, float x, float y) : obj(obj), x(x), y(y) {}
    void execute() {
        l = obj.getCameraScale();
        obj.setCameraScale(x, y);
    }
    void undo() { obj.setCameraScale(l.x, l.y); }
};

class SetCameraShear : public Command {
protected:
    Projector& obj;
    vector<float> v;
    vector<float> l;
public:
    SetCameraShear(Projector& obj, vector<float>  v) : obj(obj), v(v) {}
    void execute() {
        l = obj.getCameraShear();
        obj.setCameraShear(v); }
    void undo() { obj.setCameraShear(l); }
};
