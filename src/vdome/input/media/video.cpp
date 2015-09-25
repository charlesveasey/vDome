#include "video.h"
using namespace vd;

Video::Video() {
	bEnded = false;
}

bool Video::open(string filepath){
    if (!player.load(filepath)){
        return false;
    }
    player.setUseTexture(true);
    player.play();
    player.setLoopState(loopT);
    return true;
}

void Video::update(){
	player.update();
    
    if (getIsMovieDone()) {
        bEnded = true;
        ofNotifyEvent(endEvent,bEnded,this);
    }
}

void Video::play(){
    player.play();
}

void Video::stop(){
    player.stop();
}

void Video::close(){
    player.stop();
    player.close();
}

void Video::seek(float f){
    player.setPosition(f);
}

bool Video::isPlaying(){
    return player.isPlaying();
}

bool Video::isLoaded(){
    return player.isLoaded();;
}

void Video::setLoop(bool lp){
    loopT = (lp ? OF_LOOP_NORMAL : OF_LOOP_NONE);
    player.setLoopState(loopT);
}

float Video::getPosition(){
    return player.getPosition();
}

float Video::getDuration(){
    return player.getDuration();
}

bool Video::getIsMovieDone(){
    return player.getIsMovieDone();
}

void Video::setVolume(float v){
    player.setVolume(v);
}

float Video::getWidth(){
    return player.getWidth();
}

float Video::getHeight(){
    return player.getHeight();
}

ofPixels& Video::getPixels(){
    return player.getPixels();
}

void Video::bind(){
	player.getTexture().setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
    return player.getTexture().bind();
}

void Video::unbind() {
	return player.getTexture().unbind();
}