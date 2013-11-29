#include "input.h"

void Input::init(){
}

void Input::setup(){
    // dome texture 
	texture.allocate(render.domeMaster, render.domeMaster, OF_IMAGE_COLOR);
    
    // create input
    switch(mode){
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
            capture.setDesiredFrameRate(render.frameRate);
            capture.initGrabber(render.domeMaster,render.domeMaster);
            texture = capture.getTextureReference();
            break;
        default:
            // load default image
            image.loadImage("grid.jpg");
            texture = image.getTextureReference();
            break;
    }
    
}

void Input::bind(){
    if (mode == 2)
        hap.getTexture()->bind();
    else if (mode == 3)
        syphon.bind();
    else
        texture.bind();
}

void Input::unbind(){
    if (mode == 2)
        hap.getTexture()->unbind();
    else if (mode == 3)
        syphon.unbind();
    else
        texture.unbind();
}

void Input::update(){
    
    if (mode == 1)
        video.update();
    else if (mode == 2)
        hap.update();
    else if (mode == 4)
        capture.update();
    
}


void Input::loadXML(ofXml &xml) {
    if (xml.exists("input[@mode]")) {
        string m = xml.getAttribute("input[@mode]");
        if (m == "image")        mode = 0;
        else if (m == "video")   mode = 1;
        else if (m == "hap")     mode = 2;
        else if (m == "syphon")  mode = 3;
        else if (m == "capture")  mode = 4;
    }
    setup();
}

void Input::saveXML(ofXml &xml) {
    xml.setAttribute("input[@mode]", ofToString(mode) );
}



