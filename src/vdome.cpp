#include "vdome.h"

// SETUP

void vdome::setup(){
	   
    
    // DEFAULT SETTINGS
    domeMaster = 2048;    
    frameRate = 60;
    
    
    // key control
    // 118 (v) = pFov
    keyControl = 118;
    
    
    // input
    // 0 = image
    // 1 = video
    // 2 = hap
    // 3 = syphon
    // 4 = capture
    input = 1;
    

    window.setup();
    
    // render settings
	ofSetFrameRate(frameRate);


	ofEnableDepthTest();
	//ofDisableArbTex();
    ofBackground(0,0,0);
	
    //ofEnableSmoothing();
	ofEnableNormalizedTexCoords();

    
    // create dome mesh
	hemisphere.setup();

    
    // create dome texture
	texture.allocate(domeMaster, domeMaster, OF_IMAGE_COLOR);

    
    // load shader
	shader.load("shaders/vdome.vert", "shaders/vdome.frag");

    

    // projector settings
    pCount = 3;
    for(int i=0; i<pCount; i++) {
        Projector p;
        p.setup(i);
        projectors.push_back(p);
    }
    
    
    
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
    
    
    // LOAD SETTINGS
    loadXML();
    
    
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
	projectors[i].cameraBegin();
    
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
    
	projectors[i].cameraEnd();
}



// UPDATE

void vdome::draw(){
	ofSetColor(255); 

	for(int i=0; i<pCount; i++){
        projectors[i].fboBegin();
        ofClear(0, 0, 0, 0);
        drawFbo(i);
		projectors[i].fboEnd();
	}

	for(int i=0; i<pCount; i++) {
		projectors[i].fboBind();        

		shader.begin();
		shader.setUniform1f("brightness", projectors[i].brightness);
		shader.setUniform1f("contrast", projectors[i].contrast);
		shader.setUniform1f("saturation", projectors[i].saturation);
		shader.setUniformTexture("texsampler", projectors[i].fboTexture, 0);
        projectors[i].draw();
        shader.end();
		
        projectors[i].fboUnbind();
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
        window.vSync = ofToBool( xml.getAttribute("[@vSync]") );
    
    
    if (xml.exists("input[@mode]")) {
        string mode = xml.getAttribute("input[@mode]");
        if (mode == "image")        input = 0;
        else if (mode == "video")   input = 1;
        else if (mode == "hap")     input = 2;
        else if (mode == "syphon")  input = 3;
        else if (mode == "capture")  input = 4;
    }
    
    
    if (xml.exists("window[@x]"))
        window.x = ofToInt( xml.getAttribute("window[@x]") );
    if (xml.exists("window[@y]"))
        window.y = ofToInt( xml.getAttribute("window[@y]") );
    if (xml.exists("window[@width]"))
        window.width = ofToInt( xml.getAttribute("window[@width]") );
    if (xml.exists("window[@height]"))
        window.height = ofToInt( xml.getAttribute("window[@height]") );

    
    if (xml.exists("projectors[@count]"))
        pCount = ofToInt( xml.getAttribute("projectors[@count]") );

    
    // global
    for(int i=0; i<pCount; i++) {
        if (xml.exists("global/projector[@azimuth]"))
            projectors[i].azimuth  = ofToInt( xml.getAttribute("global/projector[@azimuth]") );
        if (xml.exists("global/projector[@brightness]"))
            projectors[i].brightness = ofToInt( xml.getAttribute("global/projector[@brightness]") );
        if (xml.exists("global/projector[@contrast]"))
            projectors[i].contrast = ofToInt( xml.getAttribute("projectors/projector[@contrast]") );
        if (xml.exists("global/projector[@saturation]"))
            projectors[i].saturation = ofToInt( xml.getAttribute("global/projector[@saturation]") );
    }
    
    // local
    for(int i=0; i<pCount; i++) {
        if (xml.exists("projectors/projector["+ofToString(i)+"][@azimuth]"))
            projectors[i].azimuth = ofToInt( xml.getAttribute("projectors/projector["+ofToString(i)+"][@azimuth]") );
        if (xml.exists("projectors/projector["+ofToString(i)+"][@brightness]"))
            projectors[i].brightness = ofToInt( xml.getAttribute("projectors/projector["+ofToString(i)+"][@brightness]") );
        if (xml.exists("projectors/projector["+ofToString(i)+"][@contrast]"))
            projectors[i].contrast = ofToInt( xml.getAttribute("projectors/projector["+ofToString(i)+"][@contrast]") );
        if (xml.exists("projectors/projector["+ofToString(i)+"][@saturation]"))
            projectors[i].saturation = ofToInt( xml.getAttribute("projectors/projector["+ofToString(i)+"][@saturation]") );
    }

    
}







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
                    //pFov += 1;
                   // cameras[0].setFov(pFov);
                    break;
                 case 1:
                    //pLensOffsetY += .01;
                    //cameras[0].setLensOffset(ofVec2f(pLensOffsetX,pLensOffsetY));
                    break;
            }
            
            break;
        
        
        
        case OF_KEY_DOWN:
            
            switch(keyControl){
                case 118:
                    //pFov -= 1;
                    //cameras[0].setFov(pFov);
                    break;
                case 97:
                    break;
                case 101:
                    break;
                case 1:
                    //pLensOffsetY -= .01;
                    //cameras[0].setLensOffset(ofVec2f(pLensOffsetX,pLensOffsetY));
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


