#include "videoWMF.h"
using namespace vd;

VideoWMF::VideoWMF() {
	bLoop = false;
	markEnd = false;
	positionRequest = -1;
}

bool VideoWMF::open(string filepath){
	bLoaded = false;
	bSupported = false;
	bEnded = false;
	markEnd = false;
	positionRequest = -1;
	player.loadMovie(filepath);
    play();
	player.setLoop(bLoop);
    return true;
}

void VideoWMF::update(){

	if (positionRequest > -1) {
		player.setPosition(positionRequest);
		positionRequest = -1;
	}

	player.update();

	if (player.getDuration() > 0 && bLoaded == false) {
		bool b = true;
		videoLoaded(b);
	}

	if (markEnd){
		bEnded = true;
		markEnd = false;
	}	
	
	if (player.getPosition() > 0.99){
		markEnd = true;
	}
	else {
		markEnd = false;
	}

	if (bPaused == true) {
		player.pause();
	}
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
	positionRequest = f;
	player.pause();
}

bool VideoWMF::isPlaying(){
    return player.isPlaying();
}

void VideoWMF::setLoop(bool lp){
    bLoop = lp;
    player.setLoop(lp);
}

float VideoWMF::getPosition(){
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