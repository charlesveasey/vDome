#pragma once

#include "vdome.h"

void vdome::setup(){
	
	ofBackground(0,0,0);
	ofSetFrameRate(30);
	ofSetVerticalSync(false);
	ofEnableSmoothing();
	ofEnableDepthTest();
	ofEnableNormalizedTexCoords();
	ofDisableArbTex();

	hemisphere.setup();

	texture.allocate(2048, 2048, OF_IMAGE_COLOR);
	
	texture.loadImage("grid.jpg");
	//texture.loadImage("fisheye-wfcam-1600px.jpg");
	
	texture.mirror(true, false);
	
	keystone.setup(); //initializates ofxGLWarper
	keystone.activate();// this allows ofxGLWarper to automatically listen to the mouse and keyboard input and updates automatically it's matrixes.
	shader.load("shaders/vdome.vert", "shaders/vdome.frag");

	cameraCount = 1;
	cameraRotations.push_back(-72);
	cameraRotations.push_back(72);
	cameraRotations.push_back(120);
	cameraRotations.push_back(220);
	cameraRotations.push_back(270);
	cameraRotations.push_back(320);

	for(int i=0; i<cameraCount; i++) {
		ofCamera cam;
		cam.setScale(1,-1,1);
		//cam.lookAt(ofVec3f(0,0,0));
		cam.tilt(-32);
		cam.setPosition(ofVec3f(0,0,0));
		cam.rotate(cameraRotations[i], 0, 1, 0);
		cam.setFov(65);
		cameras.push_back(cam);
		
		ofRectangle rect;
		rect.setWidth(1024);
		rect.setHeight(768);
		views.push_back(rect);

		ofFbo fbo;
		fbo.allocate(1024, 768, GL_RGBA);
		fbo.begin();
		ofClear(255);
		fbo.end();
		fbos.push_back(fbo);

		ofPlanePrimitive plane;
		plane.set(1024, 768);
		planes.push_back(plane);
	}
}

float ff=0;
void vdome::update() {
	//cameras[0].rotate(ff+=.001, 0, 1, 0);
}	


void vdome::drawFbo(int i){	
	cameras[i].begin(views[i]);
	ofSetColor(255);
	texture.getTextureReference().bind();
	ofScale(20,20,20);
	hemisphere.draw();
	texture.getTextureReference().unbind();
	cameras[i].end();
}

void vdome::draw(){	
	ofSetColor(255); 



	for(int i=0; i<cameraCount; i++){
		fbos[i].begin();
			ofClear(0, 0, 0, 0);
			drawFbo(i);
		fbos[i].end();
	}

	for(int i=0; i<cameraCount; i++) {
		keystone.begin();
		keystone.draw();

		ofSetColor(255);
		
		fbos[i].getTextureReference().bind();
		
		planes[i].mapTexCoordsFromTexture(fbos[i].getTextureReference());
		planes[i].setPosition(1024 * i + 1024/2, 768/2, 0);	

		//shader.begin();				
		//shader.setUniform1f("brightness", 1);
		//shader.setUniform1f("contrast", 1);
		//shader.setUniform1f("saturation", 1);
		//shader.setUniformTexture("texsampler", fbos[i].getTextureReference(), 0);
		

		planes[i].draw();


		//planes[i].drawWireframe();
		
		//shader.end();
		
		fbos[i].getTextureReference().unbind();	
		
		
		keystone.end();
	}

	for(int i=0; i<cameraCount; i++) {
		//fbos[i].draw(1024 * i, 0);
	}

    ofSetHexColor(0xFFFFFF);
    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
}
