#include "videoWin.h"
using namespace vd;

VideoWin::VideoWin() {
    forceVideoRenderer = "";
    bLoop = false;
	bLoaded = false;
    fPath = "";
}

void VideoWin::open(string filepath){
    bEnd = false;
    fPath = filepath;
	bLoaded = false;
    if (forceVideoRenderer.length() > 0){
        vType = parseForceRenderer(forceVideoRenderer);
        switch (vType) {
            case WMF: vWMF.open(filepath); break;
            case DS:  bLoaded = vDS.open(filepath); break;
        }
    }
    else{
        vType = WMF;
        vWMF.open(filepath);
    }
}

void VideoWin::update(){
    switch (vType) {
        case WMF: 
			if (vWMF.isLoaded()){
				if (vWMF.isSupported()){
					bLoaded = true;
				}
				else if (parseForceRenderer(forceVideoRenderer) != WMF){
					vType = DS;
					bLoaded = vDS.open(fPath);
					return;
				}
			}
			vWMF.update(); break;
        case DS:  vDS.update(); break;
    }
    if (getIsMovieDone()) {
        bEnd = true;
        ofNotifyEvent(endEvent,bEnd,this);
    }
}

void VideoWin::play(){
    switch (vType) {
        case WMF: vWMF.play(); break;
        case DS:  vDS.play(); break;
    }
}

void VideoWin::stop(){
    switch (vType) {
        case WMF: vWMF.stop(); break;
        case DS:  vDS.stop(); break;
    }
}

void VideoWin::close(){
    switch (vType) {
        case WMF: vWMF.close(); break;
        case DS:  vDS.close(); break;
    }
}

void VideoWin::seek(float f){
    switch (vType) {
        case WMF: vWMF.seek(f); break;
        case DS:  vDS.seek(f);  break;
    }
}

bool VideoWin::isPlaying(){
    bool val;
    switch (vType) {
        case WMF: val = vWMF.isPlaying(); break;
        case DS:  val = vDS.isPlaying(); break;
    }
    return val;
}

void VideoWin::setLoop(bool lp){
    bLoop = lp;
    vWMF.setLoop(lp);
    vDS.setLoop(lp);
}

float VideoWin::getPosition(){
    float val;
    switch (vType) {
        case WMF: val = vWMF.getPosition(); break;
        case DS:  val = vDS.getPosition(); break;
    }
    return val;
}

float VideoWin::getDuration(){
    float val;
    switch (vType) {
        case WMF: val = vWMF.getDuration(); break;
        case DS:  val = vDS.getDuration(); break;
    }
    return val;
}

bool VideoWin::getIsMovieDone(){
    bool val;
    switch (vType) {
        case WMF: val = vWMF.getIsMovieDone(); break;
        case DS:  val = vDS.getIsMovieDone(); break;
    }
    return val;
}

void VideoWin::setVolume(float v){
    switch (vType) {
        case WMF: vWMF.setVolume(v); break;
        case DS:  vDS.setVolume(v);  break;
    }
}

bool VideoWin::isLoaded(){
    return bLoaded;
}

VideoWin::VideoTypes VideoWin::parseForceRenderer(string renderer){
    VideoTypes type;
    renderer = ofToLower(renderer);
    if (renderer == "wmf")
        type = WMF;
    else
        type = DS;
    return type;
}

float VideoWin::getWidth(){
    float val;
    switch (vType) {
        case WMF: val = vWMF.getWidth(); break;
        case DS:  val = vDS.getWidth();  break;
    }
    return val;
}

float VideoWin::getHeight(){
    float val;
    switch (vType) {
        case WMF: val = vWMF.getHeight(); break;
        case DS:  val = vDS.getHeight();  break;
    }
    return val;
}

ofPixels& VideoWin::getPixels() {
	switch (vType) {
		//case WMF: return vWMF.getPixels(); //fix
		case DS:  return vDS.getPixels();
	}
}

void VideoWin::bind() {
	switch (vType) {
		case WMF: return vWMF.bind();
		case DS:  return vDS.bind();
	}
}

void VideoWin::unbind() {
	switch (vType) {
		case WMF: return vWMF.unbind();
		case DS:  return vDS.unbind();
	}
}
