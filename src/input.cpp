#include "input.h"

void Input::init(){
}

void Input::setup(){

    texture.clear();
	texture.allocate(render.domeMaster, render.domeMaster, OF_IMAGE_COLOR);
    
    stop();
    close();
    
    // create input
    switch(mode){
        case 1: // video
            video.setPixelFormat(OF_PIXELS_RGB);
            video.loadMovie("test.mov"); 
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
            hap.loadMovie("hap.mov", OF_QTKIT_DECODE_TEXTURE_ONLY);
            hap.play();
            break;
        case 4: // syphon
           syphon.setup();
           syphon.setApplicationName("Simple Server");
           syphon.setServerName("");
            break;
        default:
            // load default image
            image.loadImage("grid.jpg");
            texture = image.getTextureReference();
            break;
    }
    
}

void Input::stop() {
    video.stop();
   // hap.stop();
}

void Input::close() {
    image.clear();
    
    video.stop();
    video.close();
    
  //  hap.stop();
  //  hap.close();
    
    capture.close();
}




void Input::bind(){
   // if (mode == 3)
   //     hap.getTexture()->bind();
   // else if (mode == 4)
   //     syphon.bind();
   // else
        texture.bind();
}

void Input::unbind(){
    //if (mode == 3)
    //    hap.getTexture()->unbind();
    //else if (mode == 4)
    //    syphon.unbind();
    //else
        texture.unbind();
}

void Input::update(){
    
    if (mode == 1)
        video.update();
    //else if (mode == 3)
    //    hap.update();
    else if (mode == 2)
        capture.update();
    
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
    xml.setAttribute("input[@mode]", ofToString(mode) );
}
