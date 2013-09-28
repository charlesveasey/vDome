#pragma once

#include "ofMain.h"
#include "hemisphere.h"
#include "ofxGLWarper.h"

class vdome : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		void drawFbo(int i);


		ofShader shader;

		ofImage texture;
		Hemisphere hemisphere;

		vector<ofCamera> cameras;
		vector<ofRectangle> views;
		vector<ofFbo>fbos;
		vector<ofPlanePrimitive> planes;

		ofxGLWarper keystone;

		int cameraCount;
		vector<double> cameraRotations;

};
	
