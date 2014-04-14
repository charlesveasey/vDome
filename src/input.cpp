#include "input.h"

/******************************************
 
 CONSTRUCTOR
 
 ********************************************/

Input::Input(){
	#ifdef TARGET_WIN32
		ofPtr <ofBaseVideoPlayer> ptr(new ofDirectShowPlayer());
		video.setPlayer(ptr);
	#endif

    maxSource = 2;
	#ifdef TARGET_OSX
		maxSource = 4;
	#endif
    
    resolution = 2048;
    frameRate = 60;
}

/******************************************
 
 SETUP
 
 ********************************************/

void Input::setup(){

    texture.clear();
    
    if (texture.getWidth() != resolution || texture.getHeight() != resolution)
        texture.allocate(resolution, resolution, OF_IMAGE_COLOR);
    
    stop();
    
    // create input
    switch(source){
        case 1: // capture
            capture.setDeviceID(0);
            capture.setDesiredFrameRate(frameRate);
            capture.initGrabber(resolution, resolution);
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
    //capture.close();
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
		if (source == 3)
			hap.getTexture()->bind();
		else if (source == 4)
			syphon.bind();
    #endif
	if (source != 3 && source != 4) {
		texture.bind();
	}
}

void Input::unbind(){
	#ifdef TARGET_OSX
		if (source == 3)
			hap.getTexture()->unbind();
		else if (source == 4)
			syphon.unbind();
    #endif
	if (source != 3 && source != 4) {
		texture.unbind();
	}
}

/******************************************
 
 UPDATE
 
 ********************************************/

void Input::update(){
    if (source == 2)
        video.update();
	else if (source == 1)
        capture.update();
	#ifdef TARGET_OSX
		if (source == 3)
		    hap.update();
    #endif
}

/******************************************
 
 KEYBOARD
 
 ********************************************/

void Input::keyPressed(int key) {
    switch (key) {
        case OF_KEY_RIGHT:  // up = switch on mode
			if (source+1 > maxSource)
				source = maxSource;
			else 
				source++;
			setup();
            break;
        case OF_KEY_LEFT:  // up = switch on mode
			if (source-1 < 0)
				source = 0;
			else
				source--;
            setup();
			break;
    }
    
}

/******************************************
 
 SETTINGS
 
 ********************************************/

void Input::loadXML(ofXml &xml) {
    
    if (xml.exists("input[@resolution]"))
        resolution = ofToInt( xml.getAttribute("input[@resolution]") );
    
    if (xml.exists("input[@mode]")) {
        string m = xml.getAttribute("input[@source]");
        if (m == "image")           source = 0;
        else if (m == "video")      source = 2;
        else if (m == "capture")    source = 1;
        else if (m == "hap")        source = 3;
        else if (m == "syphon")     source = 4;
    }
    
    setup();
}


void Input::saveXML(ofXml &xml) {
    
    xml.setAttribute("input[@resolution]", ofToString(resolution) );    
    
    string str;
    
    if (source == 0)        str = "image";
    else if (source == 2)   str = "video";
    else if (source == 1)   str = "capture";
    else if (source == 3)   str = "hap";
    else if (source == 4)   str = "capture";
    
    xml.setAttribute("input[@source]", str );
}
