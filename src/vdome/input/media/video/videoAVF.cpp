#include "videoAVF.h"
using namespace vd;

VideoAVF::VideoAVF() {
    loopT = OF_LOOP_NONE;
    bLoaded = false;
    bSupported = false;
    fCnt = 0;
    pos = 0;
    playing = false;

    ofAddListener(player.videoLoadEvent, this, &VideoAVF::videoLoaded);

    player.supportedCodecs.push_back("apch"); // pro res
    player.supportedCodecs.push_back("avc1");  // h264
}

void VideoAVF::open(string filepath){
    bLoaded = false;
    bSupported = false;
    playing = true;
    pos = 0;
    fCnt = 10;
    player.loadMovie(filepath);
    player.play();
    player.setLoopState(loopT);
    blank.allocate(128,128,GL_RGB);
    pixels.allocate(128,128, OF_IMAGE_COLOR);
    pixels.set(0);
    blank.loadData(pixels);
}

void VideoAVF::bind(){
    ofTexture tex;
    if (bSupported && (fCnt >= 10)) {
        player.getTextureReference().setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
        player.getTextureReference().bind();
    }
    else {
        blank.setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
        blank.bind();
    }
}

void VideoAVF::unbind(){
    if (bSupported && (fCnt >= 10))
        player.getTextureReference().unbind();
    else
        blank.unbind();
    if (bLoaded && bSupported && fCnt < 10)
        fCnt++;
}

void VideoAVF::update(){
    if (playing)
        player.update();
}

void VideoAVF::play(){
    player.setFrame(pos);
    player.play();
    playing = true;
}
void VideoAVF::stop(){
    pos = player.getCurrentFrame();
    playing = false;
    player.stop();
}

void VideoAVF::close(){
    player.stop();
    player.close();
    bLoaded = false;
}

void VideoAVF::seek(float f){
    player.setPosition(f);
}

bool VideoAVF::isPlaying(){
    bool bPlaying;
    bPlaying = player.isPlaying();
    return bPlaying;
}

void VideoAVF::setLoop(bool lp){
    loopT = (lp ? OF_LOOP_NORMAL : OF_LOOP_NONE);
    player.setLoopState(loopT);
}

float VideoAVF::getPosition(){
    return player.getPosition();
}

float VideoAVF::getDuration(){
    return player.getDuration();
}

bool VideoAVF::getIsMovieDone(){
    return player.getIsMovieDone();
}

bool VideoAVF::isSupported(){
    return bSupported;
}

bool VideoAVF::isLoaded(){
    return bLoaded;
}

bool VideoAVF::videoLoaded(bool &success){
    bLoaded = true;
    bSupported = success;
}

void VideoAVF::setVolume(float v){
    player.setVolume(v);
}

float VideoAVF::getWidth(){
    return player.getWidth();
}

float VideoAVF::getHeight(){
    return player.getHeight();
}