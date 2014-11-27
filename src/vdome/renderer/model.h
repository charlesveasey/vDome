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

	bool getTextureFlip();
	void setTextureFlip(bool b);

	float getTextureRotate();
	void setTextureRotate(float f);
	
	float getTextureTilt();
	void setTextureTilt(float f);
	
	float getTextureScale();
	void setTextureScale(float f);

	int editMode;
	enum editModes{NONE, T_FLIP, T_ROTATE, T_TILT, T_SCALE};

	float value;
    bool textureFlipInternal;
    float textureTiltInternal;
    float textureScaleInternal;
    float textureScaleInternalW;
    float textureScaleInternalH;

private :
    ofVboMesh vbo;
    int N;
    double radius;
    float textureScale;
    bool textureFlip;
    float textureRotate;
    float textureTilt;
};

}
