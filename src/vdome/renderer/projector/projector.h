#pragma once
#include "ofMain.h"
#include "plane.h"
#include "camera.h"
#include "command.h"
#include "mask.h"
#include "curves.h"
namespace vd {

class Projector {

public:
    
    void init(int i);
    void setup();
    void begin();
    void end();
    void bind();
    void unbind();
    void draw();
	void update();

    
    void drawPlaneConfig();
    void drawKeystone();
	void drawCurves(int x, int y);

    
    void mousePressed(ofMouseEventArgs& mouseArgs);
    void mouseDragged(ofMouseEventArgs& mouseArgs);
    void mouseReleased(ofMouseEventArgs& mouseArgs);
    
    void keyPressed(int key);
    void keyReleased(int key);
    
    void loadXML(ofXml &xml);
    void loadXML2(ofXml &xml);
    void saveXML(ofXml &xml);
    
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
    
    int index;
    bool keyboard;
    bool mouse;
    bool active;
    bool enable;
    int editMode;
    bool mod;
    bool all;
    void setValue(float v);
    Mask mask;
    ofFbo renderFbo;
    ofVboMesh renderPlane;
    CommandHistory history;

    enum editModes{ BRIGHTNESS,CONTRAST,
                    HUE, SATURATION, LIGHTNESS,
                    GAMMA, GAMMA_R, GAMMA_G, GAMMA_B,
					CURVES,
                    CORNERPIN, GRID,
                    AZIMUTH, ELEVATION, DISTANCE,
                    ROLL, TILT, PAN,
                    FOV, OFFSET_X, OFFSET_Y,
                    SCALE, SCALE_X, SCALE_Y,
                    SHEAR_XY, SHEAR_XZ, SHEAR_YX, SHEAR_YZ, SHEAR_ZX, SHEAR_ZY,
                    NONE, BRUSH_SCALE, BRUSH_OPACITY, WHITE, BLACK, ENABLE};

    // intensity
    float brightness;
    float contrast;
    int whiteLevel;
    int blackLevel;

    // color
    float hue;
    float saturation;
    float lightness;

    float gamma;
    float gammaR;
    float gammaG;
    float gammaB;

    Plane plane;
    ofTexture& getTextureReference();
    vector<ofPoint> lastKey;
    vector<ofVec3f> lastGrid;
    
    int width, height;

	// color curves
	Curves curves;

private:
    Camera camera;
    ofFbo fbo;
    ofRectangle view;

    float value;
    int fboSample;
    string xmlPrefix;
    ofVec3f sphToCar(ofVec3f t);

    // plane
    ofVec2f planePosition;

    // camera
    ofVec3f cameraPosition;
    ofVec3f cameraOrientation;
    float cameraFov;
    ofVec2f cameraOffset;
    ofVec2f cameraScale;
    vector<float> cameraShear;
    float round(float d);
    float roundTo(float val, float n);
};



/******************************************

 UNDO/REDO COMMANDS

 ********************************************/


// intensity
class SetEnable : public Command {
protected:
    Projector& obj;
    bool v;
    bool l;
public:
    SetEnable(Projector& obj, bool v) : obj(obj), v(v) {}
    void execute() {
        l = obj.enable;
        obj.enable = v; }
    void undo() { obj.enable = l; }
    void redo() { obj.enable = v; }
};
    
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
    void redo() { obj.brightness = v; }
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
    void redo() { obj.contrast = v; }
};
    
class SetBlackLevel : public Command {
protected:
    Projector& obj;
    int v;
    int l;
public:
    SetBlackLevel(Projector& obj, int v) : obj(obj), v(v) {}
    void execute() {
        l = obj.blackLevel;
        obj.blackLevel = v; }
    void undo() { obj.blackLevel = l; }
    void redo() { obj.blackLevel = v; }
};
    
class SetWhiteLevel : public Command {
protected:
    Projector& obj;
    int v;
    int l;
public:
    SetWhiteLevel(Projector& obj, int v) : obj(obj), v(v) {}
    void execute() {
        l = obj.whiteLevel;
        obj.whiteLevel = v; }
    void undo() { obj.whiteLevel = l; }
    void redo() { obj.whiteLevel = v; }
};

class SetBrushScale : public Command {
protected:
    Projector& obj;
    float v;
    float l;
public:
    SetBrushScale(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        l = obj.mask.brushScale;
        obj.mask.brushScale = v; }
    void undo() { obj.mask.brushScale = l; }
    void redo() { obj.mask.brushScale = v; }
};

class SetBrushOpacity : public Command {
protected:
    Projector& obj;
    float v;
    float l;
public:
    SetBrushOpacity(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        l = obj.mask.brushOpacity;
        obj.mask.brushOpacity = v; }
    void undo() { obj.mask.brushOpacity = l; }
    void redo() { obj.mask.brushOpacity = v; }
};

class SetBrushPoints : public Command {
protected:
    Projector& obj;
    int v;
    int l;
    CommandHistory *history;
public:
    SetBrushPoints(Projector& obj, CommandHistory *history) : obj(obj), v(v), l(l), history(history) {}
    void execute() {
        l = obj.mask.hIndex;
        v = l+1;
        obj.mask.store(v);
    }
    void undo() { obj.mask.recall(l); }
    void redo() { obj.mask.recall(v); }
};

// color
class SetHue : public Command {
protected:
    Projector& obj;
    float v;
    float l;
public:
    SetHue(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        l = obj.hue;
        obj.hue = v; }
    void undo() { obj.hue = l; }
    void redo() { obj.hue = v; }
};

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
    void redo() { obj.saturation = v; }
};

class SetLightness : public Command {
protected:
    Projector& obj;
    float v;
    float l;
public:
    SetLightness(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        l = obj.lightness;
        obj.lightness = v; }
    void undo() { obj.lightness = l; }
    void redo() { obj.lightness = v; }
};

class SetGamma : public Command {
protected:
    Projector& obj;
    float v;
    float l;
public:
    SetGamma(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        l = obj.gamma;
        obj.gamma = v; }
    void undo() { obj.gamma = l; }
    void redo() { obj.gamma = v; }
};

class SetGammaR : public Command {
protected:
    Projector& obj;
    float v;
    float l;
public:
    SetGammaR(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        l = obj.gammaR;
        obj.gammaR = v; }
    void undo() { obj.gammaR = l; }
    void redo() { obj.gammaR = v; }
};

class SetGammaG : public Command {
protected:
    Projector& obj;
    float v;
    float l;
public:
    SetGammaG(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        l = obj.gammaG;
        obj.gammaG = v; }
    void undo() { obj.gammaG = l; }
    void redo() { obj.gammaG = v; }
};

class SetGammaB : public Command {
protected:
    Projector& obj;
    float v;
    float l;
public:
    SetGammaB(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        l = obj.gammaB;
        obj.gammaB = v; }
    void undo() { obj.gammaB = l; }
    void redo() { obj.gammaB = v; }
};

// plane

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
    void redo() { obj.setKeystonePoints(v); }
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
    void redo() { obj.setGridPoints(v); }
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
    void redo() { obj.setCameraPosition(azi, ele, dis); }
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
    void redo() { obj.setCameraOrientation(roll, tilt, pan);  }
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
    void redo() { obj.setCameraFov(v); }
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
    void undo() { obj.setCameraOffset(l.x, l.y); }
    void redo() { obj.setCameraOffset(x, y);  }
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
    void redo() { obj.setCameraScale(x, y);  }
};

class SetCameraScaleX : public Command {
protected:
    Projector& obj;
    float x;
    float y;
    ofVec2f l;
public:
    SetCameraScaleX(Projector& obj, float x) : obj(obj), x(x), y(y) {}
    void execute() {
        l = obj.getCameraScale();
        y = obj.getCameraScale().y;
        obj.setCameraScale(x, y);
    }
    void undo() { obj.setCameraScale(l.x, l.y); }
    void redo() { obj.setCameraScale(x, y); }
};

class SetCameraScaleY : public Command {
protected:
    Projector& obj;
    float x;
    float y;
    ofVec2f l;
public:
    SetCameraScaleY(Projector& obj, float y) : obj(obj), x(x), y(y) {}
    void execute() {
        l = obj.getCameraScale();
        x = obj.getCameraScale().x;
        obj.setCameraScale(obj.getCameraScale().x, y);
    }
    void undo() { obj.setCameraScale(l.x, l.y); }
    void redo() { obj.setCameraScale(x, y); }
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
    void redo() { obj.setCameraShear(v); }
};

}
