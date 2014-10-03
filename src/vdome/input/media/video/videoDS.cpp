#include "videoDS.h"
using namespace vd;

VideoDS::VideoDS() {
    loopT = OF_LOOP_NONE;
	ofPtr <ofBaseVideoPlayer> ptr(new ofDirectShowPlayer());
	player.setPlayer(ptr);
	bPbo = false;
}

bool VideoDS::open(string filepath){
    if (!player.loadMovie(filepath))
        return false;
	
	player.setUseTexture(!bPbo);
    player.play();
    player.setLoopState(loopT);
    
    if (bPbo)
        pbo.allocate(2, player.getWidth(), player.getHeight());
    
    return true;
}

void VideoDS::bind(){
    if (bPbo) {
        pbo.texture.setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
        pbo.texture.bind();
    }
    else {
        player.getTextureReference().setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
        player.getTextureReference().bind();
    }
}

void VideoDS::unbind(){
    if (bPbo)
        pbo.texture.unbind();
    else
        player.getTextureReference().unbind();
}

void VideoDS::update(){
	player.update();

    if (bPbo){
		pixels = player.getPixelsRef();
		if (pixels.isAllocated())
			pbo.loadData(pixels);
    }
}

void VideoDS::play(){
    player.play();
    if (bPbo)
        pbo.startThread();
}

void VideoDS::stop(){
    if (bPbo)
        pbo.stopThread();
    player.stop();
}

void VideoDS::close(){
    if(bPbo)
        pbo.wait();
    player.stop();
    player.close();
}

void VideoDS::seek(float f){
    player.setPosition(f);
}

bool VideoDS::isPlaying(){
    bool bPlaying;
    bPlaying = player.isPlaying();
    return bPlaying;
}

void VideoDS::setUsePbo(bool uPbo){
    bPbo = uPbo;
}

void VideoDS::setLoop(bool lp){
    loopT = (lp ? OF_LOOP_NORMAL : OF_LOOP_NONE);
    player.setLoopState(loopT);
}

float VideoDS::getPosition(){
    return player.getPosition();
}

float VideoDS::getDuration(){
    return player.getDuration();
}

bool VideoDS::getIsMovieDone(){
    return player.getIsMovieDone();
}

void VideoDS::setVolume(float v){
    player.setVolume(v);
}