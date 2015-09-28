#include "videoWMF.h"
using namespace vd;

VideoWMF::VideoWMF() {
	bLoop = false;
	markEnd = false;
	positionRequestFrameCnt = 0;
	positionRequest = -1;
	storePositionFix =-1;
	seekPositionFrameCnt = 0;
	ofAddListener(player.videoLoadEvent, this, &VideoWMF::videoLoaded);
}

bool VideoWMF::open(string filepath){
	bLoaded = false;
	bSupported = false;
	bEnded = false;
	markEnd = false;
	positionRequestFrameCnt = 0;
	storePositionFrameCnt = 0;
	positionRequest = -1;
	storePositionFix = -1;
	player.loadMovie(filepath);
    play();
	player.setLoop(bLoop);
    return true;
}

void VideoWMF::update(){

	player.update();

	if (markEnd){
		bEnded = true;
		markEnd = false;
	}	
	
	if (player.getPosition() > 0.99){
		markEnd = true;
	}else {
		markEnd = false;
	}

	if (positionRequest >= 0){
		positionRequestFrameCnt++;
	}
	if (positionRequestFrameCnt > 20){
		positionRequestFrameCnt = -1;
		player.setPosition(positionRequest);
		if (bPaused) stop();
		positionRequest = -1;
		storePositionFix = -1;
	}

	if (seekPositionFrameCnt > 40)
		seekPositionFrameCnt = 0;

	if (seekPositionFrameCnt > 0)
		seekPositionFrameCnt++;
}

void VideoWMF::play(){
	bPaused = false;
	player.play();
}

void VideoWMF::stop(){
	bPaused = true;
    player.pause();
}

void VideoWMF::close(){
    player.stop();
    //player.close();
	//player.forceExit();
}

void VideoWMF::seek(float f){
	seekPositionStore = f;
	seekPositionFrameCnt = 1;
	storePositionFrameCnt = 0;
	positionRequest = f;
	storePositionFix = positionRequest;
}

bool VideoWMF::isPlaying(){
    return player.isPlaying();
}

void VideoWMF::setLoop(bool lp){
    bLoop = lp;
    player.setLoop(lp);
}

float VideoWMF::getPosition(){
	if ( seekPositionFrameCnt > 0 ) {
		return seekPositionStore;
	}
	return player.getPosition();
}

float VideoWMF::getDuration(){
    return player.getDuration();
}

bool VideoWMF::getIsMovieDone(){
	return bEnded;
}

void VideoWMF::videoLoaded(bool &success){
	bLoaded = true;
	bSupported = success;
}

bool VideoWMF::isLoaded(){
	return bLoaded;
}

bool VideoWMF::isSupported(){
	return bSupported;
}

void VideoWMF::setVolume(float v){
    player.setVolume(v);
}

float VideoWMF::getWidth(){
    return player.getWidth();
}

float VideoWMF::getHeight(){
    return player.getHeight();
}

void VideoWMF::bind() {
	return player.bind();
}

void VideoWMF::unbind() {
	return player.unbind();
}

ofPixels& VideoWMF::getPixels() {
	//fix: WMF doesn't return pixels
	ofPixels pixels;
	return pixels;
}