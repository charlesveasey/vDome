#pragma once
#include "ofMain.h"
namespace vd {

class Model {

public:
    Model();
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);
    void saveMesh(string file);

    ofVboMesh vbo;
    int N;
    double radius;
    int editMode;
    float value;
    float textureScale;
    bool textureFlip;
	bool textureFlipInternal;
    float textureRotate;
    float textureTilt;
    enum editModes{NONE, T_FLIP, T_ROTATE, T_TILT, T_SCALE};
};

}
