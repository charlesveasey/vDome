#ifdef TARGET_LINUX
#include "videoLinux.h"
using namespace vd;

VideoLinux::VideoLinux() {
    bEnd = false;
    bLoaded = false;
}

void VideoLinux::open(string filepath){
    bEnd = false;
    bLoaded = vGST.open(filepath);
}

void VideoLinux::bind(){
    vGST.bind();
}

void VideoLinux::unbind(){
    vGST.unbind();
}

void VideoLinux::update(){
    vGST.update();
    if (getIsMovieDone()) {
        bEnd = true;
        ofNotifyEvent(endEvent,bEnd,this);
    }
}

void VideoLinux::play(){
    vGST.play();
}

void VideoLinux::stop(){
    vGST.stop();
}

void VideoLinux::close(){
    vGST.close();
}

void VideoWin::seek(float f){
    vGST.seek(f);
}

bool VideoLinux::isPlaying(){
    return vGST.isPlaying();
}

void VideoLinux::setLoop(bool lp){
    vGST.setLoop(lp);
}

float VideoLinux::getPosition(){
    return vGST.getPosition();
}

float VideoLinux::getDuration(){
    return vGST.getDuration();
}

bool VideoLinux::getIsMovieDone(){
    return vGST.getIsMovieDone();
}

void VideoLinux::setVolume(float v){
    vGST.setVolume(v);
}

bool VideoLinux::isLoaded(){
    return bLoaded;
}
#endif