#include "input.h"

void Input::init(){
}

void Input::setup(){

    texture.clear();
    
    if (texture.getWidth() != render.domeMaster || texture.getHeight() != render.domeMaster)
        texture.allocate(render.domeMaster, render.domeMaster, OF_IMAGE_COLOR);
    
    stop();
    close();
    
    // create input
    switch(mode){
        case 1: // video
            video.setPixelFormat(OF_PIXELS_RGB);
            video.loadMovie("media/test.mov"); 
            texture = video.getTextureReference();
            video.play();
            break;
        case 2: // capture
            capture.setDeviceID(0);
            capture.setDesiredFrameRate(render.frameRate);
            capture.initGrabber(render.domeMaster,render.domeMaster);
            texture = capture.getTextureReference();
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
				syphon.setApplicationName("vDome");
				syphon.setServerName("capture");
			#endif
            break;
        default:
            // load default image
            image.loadImage("media/grid.jpg");
            texture = image.getTextureReference();
            break;
    }
    
}

void Input::stop() {
    video.stop();
	#ifdef TARGET_OSX
		hap.stop();
	#endif
}

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
    if (mode == 1)
        video.update();
	else if (mode == 2)
        capture.update();
	#ifdef TARGET_OSX
		if (mode == 3)
		    hap.update();
    #endif
}


void Input::loadXML(ofXml &xml) {
    if (xml.exists("input[@mode]")) {
        string m = xml.getAttribute("input[@mode]");
        if (m == "image")           mode = 0;
        else if (m == "video")      mode = 1;
        else if (m == "capture")    mode = 2;
        else if (m == "hap")        mode = 3;
        else if (m == "syphon")     mode = 4;
    }
    setup();
}


void Input::saveXML(ofXml &xml) {
    string str;
    
    if (mode == 0)        str = "image";
    else if (mode == 1)   str = "video";
    else if (mode == 2)   str = "capture";
    else if (mode == 3)   str = "hap";
    else if (mode == 4)   str = "capture";
    
    xml.setAttribute("input[@mode]", str );
}
