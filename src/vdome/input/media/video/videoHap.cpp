#include "videoHap.h"
using namespace vd;

VideoHap::VideoHap() {
    loopT = OF_LOOP_NONE;
    //shader.load("settings/shaders/hap");
}

bool VideoHap::open(string filepath){
    bEnd = false;
    if (!player.loadMovie(filepath)) {
        return false;
    }
    player.play();
    player.setLoopState(loopT);
    return true;
}

void VideoHap::bind(){
    player.getTexture()->setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
    //shader.begin();
    player.getTexture()->bind();
}

void VideoHap::unbind(){
    player.getTexture()->unbind();
    //shader.end();
}

void VideoHap::update(){
    player.update();
    if (player.getCurrentFrame() == player.getTotalNumFrames()-1)
        bEnd = true;
}

void VideoHap::play(){
    player.play();
}

void VideoHap::stop(){
    player.stop();
}

void VideoHap::close(){
    player.stop();
    player.close();
}

void VideoHap::seek(float f){
    player.setPosition(f);
}

bool VideoHap::isPlaying(){
    bool bPlaying;
    bPlaying = player.isPlaying();
    return bPlaying;
}

void VideoHap::setLoop(bool lp){
    loopT = (lp ? OF_LOOP_NORMAL : OF_LOOP_NONE);
    player.setLoopState(loopT);
}

float VideoHap::getPosition(){
    return player.getPosition();
}

float VideoHap::getDuration(){
    return player.getDuration();
}

bool VideoHap::getIsMovieDone(){
    return bEnd;
}

void VideoHap::setVolume(float v){
    player.setVolume(v);
}