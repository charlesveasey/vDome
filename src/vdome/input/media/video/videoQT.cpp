#include "VideoQT.h"
using namespace vd;

VideoQT::VideoQT() {
    loopT = OF_LOOP_NONE;
	bPbo = false;
}

bool VideoQT::open(string filepath){
    if (!player.loadMovie(filepath))
        return false;
	
	player.setUseTexture(!bPbo);
    player.play();
    player.setLoopState(loopT);
    
    if (bPbo)
        pbo.allocate(2, player.getWidth(), player.getHeight());
    
    return true;
}

void VideoQT::bind(){
    if (bPbo) {
        pbo.texture.setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
        pbo.texture.bind();
    }
    else {
        player.getTextureReference().setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
        player.getTextureReference().bind();
    }
}

void VideoQT::unbind(){
    if (bPbo)
        pbo.texture.unbind();
    else
        player.getTextureReference().unbind();
}

void VideoQT::update(){
	player.update();
    
    if (bPbo){
		pixels = player.getPixelsRef();
		if (pixels.isAllocated())
			pbo.loadData(pixels);
    }
}

void VideoQT::play(){
    player.play();
    if (bPbo)
        pbo.startThread();
}

void VideoQT::stop(){
    if (bPbo)
        pbo.stopThread();
    player.stop();
}

void VideoQT::close(){
    if(bPbo)
        pbo.wait();
    player.stop();
    player.close();
}

void VideoQT::seek(float f){
    player.setPosition(f);
}

bool VideoQT::isPlaying(){
    bool bPlaying;
    bPlaying = player.isPlaying();
    return bPlaying;
}

void VideoQT::setUsePbo(bool uPbo){
    bPbo = uPbo;
}

void VideoQT::setLoop(bool lp){
    loopT = (lp ? OF_LOOP_NORMAL : OF_LOOP_NONE);
    player.setLoopState(loopT);
}

float VideoQT::getPosition(){
    return player.getPosition();
}

float VideoQT::getDuration(){
    return player.getDuration();
}

bool VideoQT::getIsMovieDone(){
    return player.getIsMovieDone();
}

void VideoQT::setVolume(float v){
    player.setVolume(v);
}

float VideoQT::getWidth(){
    return player.getWidth();
}

float VideoQT::getHeight(){
    return player.getHeight();
}