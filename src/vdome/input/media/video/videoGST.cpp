#ifdef TARGET_LINUX
#include "videoGST.h"
using namespace vd;

VideoGST::VideoGST() {
    bPbo = false;
    loopT = OF_LOOP_NONE;
}

bool VideoGST::open(string filepath){
    mutex.lock();
    if (!player.loadMovie(filepath)) {
        mutex.unlock();
        return false;
    }
    player.setUseTexture(false);
    player.play();
    player.setLoopState(loopT);
    mutex.unlock();

    if (bPbo)
        pbo.allocate(2);
    else
        texture.allocate(player.getWidth(), player.getHeight(), GL_RGB);

    startThread();
    return true;
}

void VideoGST::bind(){
    ofScopedLock lock(mutex);
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
    ofScopedLock lock(mutex);
    if (bPbo)
        pbo.texture.unbind();
    else
        texture.unbind();
}

void VideoGST::update(){
    ofScopedLock lock(mutex);
    if (bPbo && pixels.isAllocated() && player.isPlaying()){
        pbo.loadData(pixels);
    }
    else if (pixels.isAllocated() && player.isPlaying()) {
        texture.loadData(pixels);
    }
}

void VideoGST::threadedFunction(){
    while (isThreadRunning()){
        mutex.lock();
        player.update();
        pixels = player.getPixelsRef();
        mutex.unlock();
    }
}

void VideoGST::play(){
    ofScopedLock lock(mutex);
    player.play();
    if (bPbo)
        pbo.startThread();
    startThread();
}

void VideoGST::stop(){
    if (bPbo)
        pbo.stopThread();
    stopThread();
    player.stop();
}

void VideoGST::close(){
    if(bPbo)
        pbo.wait();
    waitForThread(true);
    player.stop();
    player.close();
}

void VideoGST::seek(float f){
    player.setPosition(f);
}

bool VideoGST::isPlaying(){
    ofScopedLock lock(mutex);
    bool bPlaying;
    bPlaying = player.isPlaying();
    return bPlaying;
}

void VideoGST::setUsePbo(bool uPbo){
    ofScopedLock lock(mutex);
    bPbo = uPbo;
}

void VideoGST::setLoop(bool lp){
    loopT = (lp ? OF_LOOP_NORMAL : OF_LOOP_NONE);
    ofScopedLock lock(mutex);
    player.setLoopState(loopT);
}

float VideoGST::getPosition(){
    ofScopedLock lock(mutex);
    return player.getPosition();
}

float VideoGST::getDuration(){
    ofScopedLock lock(mutex);
    return player.getDuration();
}

bool VideoGST::getIsMovieDone(){
    ofScopedLock lock(mutex);
    return player.getIsMovieDone();
}

void VideoGST::setVolume(float v){
    player.setVolume(v);
}
#endif
