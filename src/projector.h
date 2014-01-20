#pragma once

#include "ofMain.h"
#include "plane.h"
#include "camera.h"
#include "command.h"

class Projector {
    
private:
    camera camera;
    ofFbo fbo;
    ofRectangle view;
    Plane plane;

    float value;
    
    string xmlPrefix;
    ofVec3f sphToCar(ofVec3f t);
    
    
    // intensity
    float brightness;
    float contrast;
    
    // color
    float saturation;
    
    
    // plane position
    float x;
    float y;
    
    // plane dimensions
    float width;
    float height;
    
    // plane scale
    float scaleX;
    float scaleY;
    
    
    // camera position
    float azimuth;
    float elevation;
    float distance;
    
    // camera orientation
    float roll;
    float tilt;
    float pan;
    
    // camera lens
    float fov;
    float offsetX;
    float offsetY;
    
    // camera shear
    float shearXY;
    float shearXZ;
    float shearYX;
    float shearYZ;
    float shearZX;
    float shearZY;
    

public:
    int index;
    bool keyboard;
    bool mouse;
    CommandHistory history;
    int editMode;
    bool mod;
    
    void setValue(float v);
    
    
    // keystone flag
    bool getKeystoneActive();
    void setKeystoneActive(bool v);
    
    
    // grid flag
    bool getGridActive();
    void setGridActive(bool v);
        
    
    // intensity
    float getBrightness();
    void  setBrightness(float v);
    
    float getContrast();
    void  setContrast(float v);
    
    
    // color
    float getSaturation();
    void  setSaturation(float v);
    
    
    
    // plane position
    float getX();
    void  setX(float v);
    
    float getY();
    void  setY(float v);

    
    // plane dimensions
    float getWidth();
    void  setWidth(float v);
    
    float getHeight();
    void  setHeight(float v);
    
    
    // plane scale
    void  setScale(float vx, float vy);
    
    float getScaleX();
    void  setScaleX(float v);
    
    float getScaleY();
    void  setScaleY(float v);
    
    
    
    // camera position
    void  setCameraTransform();
    
    void  setCameraPosition(ofVec3f sph);
    
    float getAzimuth();
    void  setAzimuth(float v);
    
    float getElevation();
    void  setElevation(float v);
    
    float getDistance();
    void  setDistance(float v);
    
    
    // camera orientation
    float getRoll();
    void  setRoll(float v);
    
    float getTilt();
    void  setTilt(float v);
    
    float getPan();
    void  setPan(float v);
    
    
    // camera lens
    float getFov();
    void  setFov(float v);
    
    void  setOffset(float vx, float vy);
    
    float getOffsetX();
    void  setOffsetX(float v);
    
    float getOffsetY();
    void  setOffsetY(float v);
    
    
    
    // camera shear
    float getShearXY();
    void  setShearXY(float v);
    
    float getShearXZ();
    void  setShearXZ(float v);

    float getShearYX();
    void  setShearYX(float v);

    float getShearYZ();
    void  setShearYZ(float v);

    float getShearZX();
    void  setShearZX(float v);

    float getShearZY();
    void  setShearZY(float v);
    
    
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
    
};



// intensity
class SetBrightness : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetBrightness(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setBrightness(v);
    }
    void undo() {
        obj.setBrightness(v);
    }
};

class SetContrast : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetContrast(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setContrast(v);
    }
    void undo() {
        obj.setContrast(v);
    }
};



// color
class SetSaturation : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetSaturation(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setSaturation(v);
    }
    void undo() {
        obj.setSaturation(v);
    }
};



// plane position
class SetX : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetX(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setX(v);
    }
    void undo() {
        obj.setX(v);
    }
};

class SetY : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetY(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setY(v);
    }
    void undo() {
        obj.setY(v);
    }
};


// plane dimension
class SetWidth : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetWidth(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setWidth(v);
    }
    void undo() {
        obj.setWidth(v);
    }
};

class SetHeight : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetHeight(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setHeight(v);
    }
    void undo() {
        obj.setHeight(v);
    }
};


// plane scale
class SetScale : public Command {
protected:
    Projector& obj;
    float vx;
    float vy;
public:
    SetScale(Projector& obj, float vx, float vy) : obj(obj), vx(vx), vy(vy) {}
    void execute() {
        obj.setScale(vx, vy);
    }
    void undo() {
        obj.setScale(vx, vy);
    }
};

class SetScaleX : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetScaleX(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setScaleX(v);
    }
    void undo() {
        obj.setScaleX(v);
    }
};

class SetScaleY : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetScaleY(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setScaleY(v);
    }
    void undo() {
        obj.setScaleY(v);
    }
};



// camera position
class SetAzimuth : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetAzimuth(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setAzimuth(v);
    }
    void undo() {
        obj.setAzimuth(v);
    }
};

class SetElevation : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetElevation(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setElevation(v);
    }
    void undo() {
        obj.setElevation(v);
    }
};

class SetDistance : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetDistance(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setDistance(v);
    }
    void undo() {
        obj.setDistance(v);
    }
};


// camera orientation
class SetRoll : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetRoll(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setRoll(v);
    }
    void undo() {
        obj.setRoll(v);
    }
};

class SetTilt : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetTilt(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setTilt(v);
    }
    void undo() {
        obj.setTilt(v);
    }
};

class SetPan : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetPan(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setPan(v);
    }
    void undo() {
        obj.setPan(v);
    }
};


// camera lens
class SetFov : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetFov(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setFov(v);
    }
    void undo() {
        obj.setFov(v);
    }
};

class SetOffsetX : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetOffsetX(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setOffsetX(v);
    }
    void undo() {
        obj.setOffsetX(v);
    }
};

class SetOffsetY : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetOffsetY(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setOffsetY(v);
    }
    void undo() {
        obj.setOffsetY(v);
    }
};


// camera shear
class SetShearXY : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetShearXY(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setShearXY(v);
    }
    void undo() {
        obj.setShearXY(v);
    }
};

class SetShearXZ : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetShearXZ(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setShearXZ(v);
    }
    void undo() {
        obj.setShearXZ(v);
    }
};

class SetShearYX : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetShearYX(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setShearYX(v);
    }
    void undo() {
        obj.setShearYX(v);
    }
};

class SetShearYZ : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetShearYZ(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setShearYZ(v);
    }
    void undo() {
        obj.setShearYZ(v);
    }
};

class SetShearZX : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetShearZX(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setShearZX(v);
    }
    void undo() {
        obj.setShearZX(v);
    }
};

class SetShearZY : public Command {
protected:
    Projector& obj;
    float v;
public:
    SetShearZY(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        obj.setShearZY(v);
    }
    void undo() {
        obj.setShearZY(v);
    }
};