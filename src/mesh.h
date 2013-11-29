#pragma once

#include "ofMain.h"

class Mesh {
	
public:

    void init();
    void setup();

    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);
    
    void update();
    void draw();
				
    ofVboMesh vbo;

    int N;
    double radius;
    
};
	