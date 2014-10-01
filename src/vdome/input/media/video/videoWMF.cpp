#include "videoWMF.h"
using namespace vd;

VideoWMF::VideoWMF() {
	bLoop = false;
	markEnd = false;
	ofAddListener(player.videoLoadEvent, this, &VideoWMF::videoLoaded);
}

bool VideoWMF::open(string filepath){
	bLoaded = false;
	bSupported = false;
	bEnded = false;
	markEnd = false;
	player.loadMovie(filepath);
    player.play();
	player.setLoop(bLoop);
    return true;
}

void VideoWMF::bind(){
	player.bind();
}

void VideoWMF::unbind(){
	player.unbind();
}
void VideoWMF::update(){
	player.update();
	if (markEnd && !player.getPosition()){
		bEnded = true;
		markEnd = false;
	}	
	if (player.getPosition() > 0.99){
		markEnd = true;
	}else {
		markEnd = false;
	}
}

void VideoWMF::play(){
    player.play();
}

void VideoWMF::stop(){
    player.stop();
}

void VideoWMF::close(){
    player.stop();
    //player.close();
	//player.forceExit();
}

void VideoWMF::seek(float f){
    player.setPosition(f);
}

bool VideoWMF::isPlaying(){
    bool bPlaying;
    bPlaying = player.isPlaying();
    return bPlaying;
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