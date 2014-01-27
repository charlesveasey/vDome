#include "input.h"

#ifdef TARGET_WIN32
	#include "ofDirectShowPlayer.h"
#endif

/******************************************
 
 CONSTRUCTOR
 
 ********************************************/

Input::Input(){
	#ifdef TARGET_WIN32
		ofPtr <ofBaseVideoPlayer> ptr(new ofDirectShowPlayer());
		video.setPlayer(ptr);
	#endif

    maxMode = 2;
	#ifdef TARGET_OSX
		maxMode = 4;
	#endif
    
    domeMaster = 2048;
    frameRate = 60;
}


/******************************************
 
 SETUP
 
 ********************************************/

void Input::setup(){

    texture.clear();
    
    if (texture.getWidth() != domeMaster || texture.getHeight() != domeMaster)
        texture.allocate(domeMaster, domeMaster, OF_IMAGE_COLOR);
    
    stop();
    
    // create input
    switch(mode){
        case 1: // capture
            capture.setDeviceID(0);
            capture.setDesiredFrameRate(frameRate);
            capture.initGrabber(domeMaster, domeMaster);
            texture = capture.getTextureReference();
            break;
        case 2: // video
            video.setPixelFormat(OF_PIXELS_RGB);
			#ifdef TARGET_WIN32
				video.loadMovie("media/test.avi"); 
			#endif
			#ifdef TARGET_OSX
				video.loadMovie("media/test.mov"); 
			#endif
            texture = video.getTextureReference();
            video.play();
            break;
        case 3: // hap video
			#ifdef TARGET_OSX
				hap.loadMovie("media/hap.mov", OF_QTKIT_DECODE_TEXTURE_ONLY);
				hap.play();
			#endif
            break;
        case 4: // syphon
			#ifdef TARGET_OSX
				syphon.setup();
			#endif
            break;
        default:
            // load default image
            image.loadImage("media/grid.jpg");
            texture = image.getTextureReference();
            break;
    }
    
}

/******************************************
 
 STOP
 
 ********************************************/

void Input::stop() {
    video.stop();
	#ifdef TARGET_OSX
		hap.stop();
	#endif
}

/******************************************
 
 CLOSE
 
 ********************************************/

void Input::close() {
    image.clear();
    video.stop();
    video.close();
    capture.close();
    #ifdef TARGET_OSX
		hap.stop();
		hap.close();
    #endif
}

/******************************************
 
 BIND
 
 ********************************************/

void Input::bind(){
	#ifdef TARGET_OSX
		if (mode == 3)
			hap.getTexture()->bind();
		else if (mode == 4)
			syphon.bind();
    #endif
	if (mode != 3 && mode != 4) {
		texture.bind();
	}
}

void Input::unbind(){
	#ifdef TARGET_OSX
		if (mode == 3)
			hap.getTexture()->unbind();
		else if (mode == 4)
			syphon.unbind();
    #endif
	if (mode != 3 && mode != 4) {
		texture.unbind();
	}
}

void Input::update(){
    if (mode == 2)
        video.update();
	else if (mode == 1)
        capture.update();
	#ifdef TARGET_OSX
		if (mode == 3)
		    hap.update();
    #endif
}

/******************************************
 
 KEYBOARD
 
 ********************************************/

void Input::keyPressed(int key) {
    switch (key) {
        case OF_KEY_UP:  // up = switch on mode
			if (mode+1 > maxMode)
				mode = maxMode;
			else 
				mode++;
			setup();
            break;
        case OF_KEY_DOWN:  // up = switch on mode
			if (mode-1 < 0)
				mode = 0;
			else
				mode--;
            setup();
			break;
    }
    
}

/******************************************
 
 SETTINGS
 
 ********************************************/

void Input::loadXML(ofXml &xml) {
    
    if (xml.exists("input[@domeMaster]"))
        domeMaster = ofToInt( xml.getAttribute("input[@domeMaster]") );
    
    if (xml.exists("input[@mode]")) {
        string m = xml.getAttribute("input[@mode]");
        if (m == "image")           mode = 0;
        else if (m == "video")      mode = 2;
        else if (m == "capture")    mode = 1;
        else if (m == "hap")        mode = 3;
        else if (m == "syphon")     mode = 4;
    }
    
    setup();
}


void Input::saveXML(ofXml &xml) {
    
    xml.setAttribute("input[@domeMaster]", ofToString(domeMaster) );    
    
    string str;
    
    if (mode == 0)        str = "image";
    else if (mode == 2)   str = "video";
    else if (mode == 1)   str = "capture";
    else if (mode == 3)   str = "hap";
    else if (mode == 4)   str = "capture";
    
    xml.setAttribute("input[@mode]", str );
}
