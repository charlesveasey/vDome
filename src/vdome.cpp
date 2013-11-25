#include "vdome.h"

// SETUP

void vdome::setup(){
	   
    
    // DEFAULT SETTINGS
    
    // input
    // 0 = image
    // 1 = video
    input = 1;
    
    // window settings
    wX = 0;
    wY = 0;
    wWidth = 2048;
    wHeight = 768;
    
    // render settings
    vSync = false;
    frameRate = 60;
    domeMasterRes = 2048;
    
    // projector settings
    pCount = 3;
    pWidth = 1024;
    pHeight = 768;
    pElevation = 0;
    pDistance = 5;
    pTilt = -16;
    pFov = 45;
    pLensOffsetX = 0;
    pLensOffsetY = .31;
    
    pAzimuth.push_back(0);
    pAzimuth.push_back(-45);
    pAzimuth.push_back(-90);
    
    pBrightness.push_back(1);
    pBrightness.push_back(1);
    pBrightness.push_back(1);
    
    pContrast.push_back(1);
    pContrast.push_back(1);
    pContrast.push_back(1);
   
    pSaturation.push_back(1);
    pSaturation.push_back(1);
    pSaturation.push_back(1);
   
    
    // TODO: load user settings file
    
    
    
    // RENDER
    
    // window settings
    ofSetWindowPosition(wX, wY);
    ofSetWindowShape(wWidth, wHeight);
    
    // render settings
	ofSetFrameRate(frameRate);
	ofSetVerticalSync(vSync);

	ofEnableDepthTest();
	//ofDisableArbTex();
    ofBackground(0,0,0);
	
    //ofEnableSmoothing();
	ofEnableNormalizedTexCoords();

    
    // create dome mesh
	hemisphere.setup();

    
    // create dome texture
	texture.allocate(domeMasterRes, domeMasterRes, OF_IMAGE_COLOR);

    // create input
    if (input == 1) {
        // load default movie
        video.setPixelFormat(OF_PIXELS_RGB);
        video.loadMovie("test.mov");
        texture = video.getTextureReference();
        video.play();
    }
    else {
        // load default image
        image.loadImage("grid.jpg");
        texture = image.getTextureReference();
    }
    
    
    // load shader
	shader.load("shaders/vdome.vert", "shaders/vdome.frag");

    
    
    
    // create projectors
	for(int i=0; i<pCount; i++) {
        
        // convert camera position -> spherical to cartesian
        ofVec3f sph, car;
        sph.set(pAzimuth[i], pElevation, pDistance);
        car = sphToCar(sph);
        
        // create camera
        ofCamera cam;
		cam.setScale(1,-1,1); // legacy oF oddity
        cam.setFov(pFov);
		cam.setPosition(car);
		cam.rotate(pAzimuth[i], 0, 1, 0);
        cam.tilt(pTilt);
        cam.setLensOffset(ofVec2f(pLensOffsetX,pLensOffsetY));
        cameras.push_back(cam);
		
        // create view
		ofRectangle rect;
		rect.setWidth(pWidth);
		rect.setHeight(pHeight);
		views.push_back(rect);

        // create fbo
		ofFbo fbo;
		fbo.allocate(pWidth, pHeight, GL_RGBA);
		fbo.begin();
		ofClear(255);
		fbo.end();
		fbos.push_back(fbo);

        // create render plane
		ofPlanePrimitive plane;
		plane.set(pWidth, pHeight);
		planes.push_back(plane);
	}
}


// UPDATE

float ff=0;

void vdome::update() {
    
    if (input == 1) {
        video.update();
    }

    
	//cameras[0].rotate(ff+=.001, 0, 1, 0);
    //cameras[0].setLensOffset(ofVec2f(0,ff+=.001));
}


void vdome::drawFbo(int i){
	cameras[i].begin(views[i]);
    texture.bind();
	hemisphere.draw();
    texture.unbind();
	cameras[i].end();
}



// UPDATE

void vdome::draw(){
	ofSetColor(255); 

	for(int i=0; i<pCount; i++){
		fbos[i].begin();
			ofClear(0, 0, 0, 0);
			drawFbo(i);
		fbos[i].end();
	}

	for(int i=0; i<pCount; i++) {
		
		fbos[i].getTextureReference().bind();
		planes[i].mapTexCoordsFromTexture(fbos[i].getTextureReference());
		planes[i].setPosition(pWidth * i + pWidth/2, pHeight/2, 0);

		shader.begin();
		shader.setUniform1f("brightness", pBrightness[i]);
		shader.setUniform1f("contrast", pContrast[i]);
		shader.setUniform1f("saturation", pSaturation[i]);
		shader.setUniformTexture("texsampler", fbos[i].getTextureReference(), 0);

		planes[i].draw();
		//planes[i].drawWireframe();

		shader.end();
		
		fbos[i].getTextureReference().unbind();
	}

    ofSetHexColor(0xFFFFFF);
    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
}



// UTILS

// converts spherical to cartesian coordinates
ofVec3f vdome::sphToCar(ofVec3f t) {
    float azi, ele, dis;
    float x, y, z;
    azi = ofDegToRad(t.x);
    ele = ofDegToRad(t.y+90);
    dis = t.z;
    x = sin(azi) * sin(ele) * dis;
    y = cos(ele) * dis;
    z = cos(azi) * sin(ele) * dis;
    return ofVec3f(x,y,z);
};





// EVENTs

void vdome::keyPressed(int key){
    switch(key){
        case OF_KEY_LEFT:
            cout << "keyPressed " << key << endl;
            break;
        case OF_KEY_RIGHT:
            cout << "keyPressed " << key << endl;
            break;
            
        case OF_KEY_UP:
            
            pLensOffsetY += .01;
            cameras[0].setLensOffset(ofVec2f(pLensOffsetX,pLensOffsetY));
            
            cout << "keyPressed " << key << endl;
            break;
        case OF_KEY_DOWN:
            
            pLensOffsetY -= .01;
            cameras[0].setLensOffset(ofVec2f(pLensOffsetX,pLensOffsetY));
            
            cout << "keyPressed " << key << endl;
            break;
    }
}
