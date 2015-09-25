#pragma once
#include "command.h"
#include "projector.h"
#include "input.h"

namespace vd {

/******************************************

 PROJECTOR

 ********************************************/

class SetProjectors : public Command {
protected:
     vector<Command*> cmds;
public:
    SetProjectors(vector<Command*> cmds) : cmds(cmds) {}
    void execute() {
		for (int i = 0; i < cmds.size(); i++){
			cmds.at(i)->execute();
		}
	}
    void undo() {
		for (int i = 0; i < cmds.size(); i++){
			cmds.at(i)->undo();
		}
	}
    void redo() { 		
		for (int i = 0; i < cmds.size(); i++){
			cmds.at(i)->redo();
		} 
	}
};


// enable
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
public:
    SetBrushPoints(Projector& obj) : obj(obj), v(v), l(l) {}
    void execute() {
        l = obj.mask.hIndex;
        v = l+1;
        obj.mask.store(v);
    }
    void undo() { obj.mask.recall(l); }
    void redo() { obj.mask.recall(v); }
};

class ResetBrushOpacity : public Command {
protected:
    Projector& obj;
    float v;
    float l;
	Command* cmd;
public:
    ResetBrushOpacity(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        l = obj.mask.brushOpacity;
        obj.mask.brushOpacity = v;
		cmd = new SetBrushPoints(obj);
		cmd->execute();
		obj.mask.reset();
	}
    void undo() { obj.mask.brushOpacity = l; cmd->undo();		}
    void redo() { obj.mask.brushOpacity = v; obj.mask.reset();	}
};

class ResetBrushScale : public Command {
protected:
    Projector& obj;
    float v;
    float l;
	Command* cmd;
public:
    ResetBrushScale(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        l = obj.mask.brushScale;
        obj.mask.brushScale = v;
		cmd = new SetBrushPoints(obj);
		cmd->execute();
		obj.mask.reset();
	}
    void undo() { obj.mask.brushScale = l; cmd->undo();			}
    void redo() { obj.mask.brushScale = v; obj.mask.reset();	}
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



// plane

class SetCornerpinPoints : public Command {
protected:
    Projector& obj;
    vector<ofPoint> v;
    vector<ofPoint> l;
public:
    SetCornerpinPoints(Projector& obj, vector<ofPoint> v, vector<ofPoint> l) : obj(obj), v(v), l(l) {}
    void execute() {;
        obj.setKeystonePoints(v);
    }
	void undo() { 
		bool tmp = obj.plane.cornerpinActive;
		obj.plane.cornerpinActive = true;
		obj.plane.setCornerpinPoints(l);
		obj.plane.draw();
		obj.plane.cornerpinActive = tmp;
	}
    void redo() { 
		bool tmp = obj.plane.cornerpinActive;
		obj.plane.cornerpinActive = true;
		obj.plane.setCornerpinPoints(v);
		obj.plane.draw();
		obj.plane.cornerpinActive = tmp;
	}
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
    void undo() { 
		obj.setGridPoints(l); 
	}
    void redo() { 
		obj.setGridPoints(v); 
	}
};


class ResetCornerpin : public Command {
protected:
    Projector& obj;
	Command* cmd;
	vector<ofPoint> l;
public:
    ResetCornerpin(Projector& obj) : obj(obj) {}
    void execute() {
		l = obj.getKeystonePoints();
		cmd = new SetCornerpinPoints(obj, l, l); 
        obj.plane.resetCornerpin();
    }
	void undo() { 
		cmd->undo();
	}
    void redo() { 
		obj.plane.resetCornerpin();
	}
};


class ResetGrid : public Command {
protected:
    Projector& obj;
	Command* cmd;
	vector<ofPoint> l;
public:
    ResetGrid(Projector& obj) : obj(obj) {}
    void execute() {
		l = obj.getGridPoints();
        obj.plane.resetGrid();
    }
	void undo() { 
		obj.setGridPoints(l); 
	}
    void redo() { 
		obj.plane.resetGrid();
	}
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

}
