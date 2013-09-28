#pragma once

#include "ofMain.h"

class Hemisphere {
	
	public:

		void setup();
		void update();
		void draw();
				
		ofVboMesh mesh;

		int N; 
		double radius;
};
	
