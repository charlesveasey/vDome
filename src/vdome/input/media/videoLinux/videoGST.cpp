#include "videoGST.h"
using namespace vd;

VideoGST::VideoGST() {
    bPbo = false;
    loopT = OF_LOOP_NONE;
}

bool VideoGST::open(string filepath){
    if (!player.loadMovie(filepath)) {
        return false;
    }
    player.setUseTexture(false);
    player.play();
    player.setLoopState(loopT);

    if (bPbo)
        pbo.allocate(2, player.getWidth(), player.getHeight());
    else
        texture.allocate(player.getWidth(), player.getHeight(), GL_RGB);

    return true;
}

void VideoGST::bind(){
    if (bPbo) {
        pbo.texture.setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
        pbo.texture.bind();
    }
    else {
        player.getTextureReference().setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
        player.getTextureReference().bind();
    }
}

void VideoGST::unbind(){
    if (bPbo)
        pbo.texture.unbind();
    else
        texture.unbind();
}

void VideoGST::update(){
    player.update();
    if (bPbo && pixels.isAllocated() && player.isPlaying()){
        pixels = player.getPixelsRef();
        pbo.loadData(pixels);
    }
}

void VideoGST::play(){
    player.play();
    if (bPbo)
        pbo.startThread();
}

void VideoGST::stop(){
    if (bPbo)
        pbo.stopThread();
    player.stop();
}

void VideoGST::close(){
    if(bPbo)
        pbo.wait();
    player.stop();
    player.close();
}

void VideoGST::seek(float f){
    player.setPosition(f);
}

bool VideoGST::isPlaying(){
    bool bPlaying;
    bPlaying = player.isPlaying();
    return bPlaying;
}

void VideoGST::setUsePbo(bool uPbo){
    bPbo = uPbo;
}

void VideoGST::setLoop(bool lp){
    loopT = (lp ? OF_LOOP_NORMAL : OF_LOOP_NONE);
    player.setLoopState(loopT);
}

float VideoGST::getPosition(){
    return player.getPosition();
}

float VideoGST::getDuration(){
    return player.getDuration();
}

bool VideoGST::getIsMovieDone(){
    return player.getIsMovieDone();
}

void VideoGST::setVolume(float v){
    player.setVolume(v);
}
