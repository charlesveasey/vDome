#include "vdome.h"

// SETUP

void vdome::setup(){
	   
    
    // DEFAULT SETTINGS
    
    // key control
    // 118 (v) = pFov
    keyControl = 118;
    
    
    // input
    // 0 = image
    // 1 = video
    // 2 = hap
    // 3 = syphon
    // 4 = capture
    input = 0;
    
    // window settings
    wX = 0;
    wY = 0;
    wWidth = 2048;
    wHeight = 768;
    
    // render settings
    vSync = false;
    frameRate = 60;
    domeMaster = 2048;
    
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
    
    pAzimuth.push_back(-90);
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
   
    
    
    
    // LOAD SETTINGS
    loadXML();

    
    
    
    
    
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
	texture.allocate(domeMaster, domeMaster, OF_IMAGE_COLOR);

    // create input
    switch(input){
        case 1: // video
            video.setPixelFormat(OF_PIXELS_RGB);
            video.loadMovie("test.mov");
            texture = video.getTextureReference();
            video.play();
            break;
        case 2: // hap video
            hap.loadMovie("hap.mov", OF_QTKIT_DECODE_TEXTURE_ONLY);
            hap.play();
            break;
        case 3: // syphon
            syphon.setup();
            syphon.setApplicationName("Simple Server");
            syphon.setServerName("");
            break;
        case 4: // capture
            capture.setDeviceID(0);
            capture.setDesiredFrameRate(frameRate);
            capture.initGrabber(domeMaster,domeMaster);
            texture = capture.getTextureReference();            
            break;
        default:
            // load default image
            image.loadImage("grid.jpg");
            texture = image.getTextureReference();
            break;
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
    
    if (input == 1)
        video.update();
    else if (input == 2)
        hap.update();
    else if (input == 4)
        capture.update();
    
	//cameras[0].rotate(ff+=.001, 0, 1, 0);
    //cameras[0].setLensOffset(ofVec2f(0,ff+=.001));
}


void vdome::drawFbo(int i){
	cameras[i].begin(views[i]);
    
    if (input == 2)
        hap.getTexture()->bind();
    else if (input == 3)
        syphon.bind();
    else
        texture.bind();
    
    hemisphere.draw();
   
    if (input == 2)
        hap.getTexture()->unbind();
    else if (input == 3)
        syphon.unbind();
    else
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


void vdome::loadXML() {
    xml.load("settings.xml");
    
    
    if (xml.exists("[@domeMaster]"))
        domeMaster = ofToInt( xml.getAttribute("[@domeMaster]") );
    if (xml.exists("[@frameRate]"))
        frameRate = ofToInt( xml.getAttribute("[@frameRate]") );
    if (xml.exists("[@vSync]"))
        vSync = ofToBool( xml.getAttribute("[@vSync]") );
    
    
    if (xml.exists("input[@mode]")) {
        string mode = xml.getAttribute("input[@mode]");
        if (mode == "image")        input = 0;
        else if (mode == "video")   input = 1;
        else if (mode == "hap")     input = 2;
        else if (mode == "syphon")  input = 3;
        else if (mode == "capture")  input = 4;
    }
    
    
    if (xml.exists("window[@x]"))
        wX = ofToInt( xml.getAttribute("window[@x]") );
    if (xml.exists("window[@y]"))
        wY = ofToInt( xml.getAttribute("window[@y]") );
    if (xml.exists("window[@width]"))
        wWidth = ofToInt( xml.getAttribute("window[@width]") );
    if (xml.exists("window[@height]"))
        wHeight = ofToInt( xml.getAttribute("window[@height]") );

    
    if (xml.exists("projectors[@count]"))
        pCount = ofToInt( xml.getAttribute("projectors[@count]") );

    // local
    for(int i=0; i<pCount; i++) {
        if (xml.exists("projectors/projector["+ofToString(i)+"][@azimuth]"))
            pAzimuth[i] = ofToInt( xml.getAttribute("projectors/projector["+ofToString(i)+"][@azimuth]") );
        if (xml.exists("projectors/projector["+ofToString(i)+"][@brightness]"))
            pBrightness[i] = ofToInt( xml.getAttribute("projectors/projector["+ofToString(i)+"][@brightness]") );
        if (xml.exists("projectors/projector["+ofToString(i)+"][@contrast]"))
            pContrast[i] = ofToInt( xml.getAttribute("projectors/projector["+ofToString(i)+"][@contrast]") );
        if (xml.exists("projectors/projector["+ofToString(i)+"][@saturation]"))
            pSaturation[i] = ofToInt( xml.getAttribute("projectors/projector["+ofToString(i)+"][@saturation]") );
    }
    
    // global
    for(int i=0; i<pCount; i++) {
        if (xml.exists("global/projector[@azimuth]"))
            pAzimuth[i] = ofToInt( xml.getAttribute("global/projector[@azimuth]") );
        if (xml.exists("global/projector[@brightness]"))
            pBrightness[i] = ofToInt( xml.getAttribute("global/projector[@brightness]") );
        if (xml.exists("global/projector[@contrast]"))
            pContrast[i] = ofToInt( xml.getAttribute("projectors/projector[@contrast]") );
        if (xml.exists("global/projector[@saturation]"))
            pSaturation[i] = ofToInt( xml.getAttribute("global/projector[@saturation]") );
    }
    
}


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
    
    cout << "keyPressed " << key << endl;
    
    switch(key){
          
            
            
        case OF_KEY_LEFT:
            cout << "keyPressed " << key << endl;
            break;
        case OF_KEY_RIGHT:
            cout << "keyPressed " << key << endl;
            break;
            
           
            
            
        case OF_KEY_UP:
            
            switch(keyControl){
                case 118:
                    pFov += 1;
                    cameras[0].setFov(pFov);
                    break;
                 case 1:
                    pLensOffsetY += .01;
                    cameras[0].setLensOffset(ofVec2f(pLensOffsetX,pLensOffsetY));
                    break;
            }
            
            break;
        
        
        
        case OF_KEY_DOWN:
            
            switch(keyControl){
                case 118:
                    pFov -= 1;
                    cameras[0].setFov(pFov);
                    break;
                case 97:
                    break;
                case 101:
                    break;
                case 1:
                    pLensOffsetY -= .01;
                    cameras[0].setLensOffset(ofVec2f(pLensOffsetX,pLensOffsetY));
                    break;
            }
            
            cout << "keyPressed " << key << endl;
            break;
        
            
        // key control shortcuts
        case 118: // v = fov
            keyControl = key;
            break;
        case 97: // a = azi
            keyControl = key;
            break;
        case 101: // e = ele
            keyControl = key;
            break;
            
    }
}


