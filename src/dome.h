#pragma once

#include "ofMain.h"

class Dome {
	
public:

    void init();
    void setup();

    void keyPressed(int key);
    void keyPressedInput(int key);
    
    void keyReleased(int key);
    
    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);
    
    void saveMesh(string file);
    
    void update();
    void draw();
				
    ofVboMesh vbo;

    int N;
    double radius;
    
    int editMode;
    float value;
    
    float textureScale;
};
	