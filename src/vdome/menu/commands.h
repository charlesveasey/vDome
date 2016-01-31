#pragma once
#include "command.h"
#include "projector.h"
#include "input.h"
namespace vd {

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

class SetBrightness : public Command {
protected:
    Projector& obj;
    float v;
    float l;
public:
    SetBrightness(Projector& obj, float v) : obj(obj), v(v) {}
    void execute() {
        l = obj.brightness;
        obj.setBrightness(v); }
    void undo() { obj.setBrightness(l); }
    void redo() { obj.setBrightness(l); }
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
        obj.setContrast(v); }
    void undo() { obj.setContrast(l); }
    void redo() { obj.setContrast(v); }
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
        obj.setSaturation(v); }
    void undo() { obj.setSaturation(l); }
    void redo() { obj.setSaturation(v); }
};

class ResetGrid : public Command {
protected:
    Projector& obj;
	Command* cmd;
	vector<ofPoint> l;
public:
    ResetGrid(Projector& obj) : obj(obj) {}
    void execute() {
		//l = obj.getGridPoints();
        //obj.plane.resetGrid();
        //fix
    }
	void undo() { 
	}
    void redo() { 
		//obj.plane.resetGrid();
        //fix
	}
};

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

}////////////
