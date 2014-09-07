#include "videoOSX.h"
using namespace vd;

VideoOSX::VideoOSX() {
    forceVideoRenderer = "";
    bLoop = false;
    bLoaded = false;
    fPath = "";
    bAVF = false;
}

void VideoOSX::open(string filepath){
    bEnd = false;
    bLoaded = false;
    fPath = filepath;
    if (forceVideoRenderer.length() > 0){
        vType = parseForceRenderer(forceVideoRenderer);
        switch (vType) {
            case AVF:
                vAVF.open(filepath);        break;
            case HAP: bLoaded = vHap.open(filepath);  break;
            case QT:  bLoaded = vQT.open(filepath);   break;
        }
    }
    else{ // AVF not working under 10.9, allow internal flag until fixed
        if (bAVF){
            vType = AVF;
            vAVF.open(filepath);
        }
        else{
            vType = HAP;
            if (vHap.open(fPath)){
                bLoaded = true;
            }
            else{
                vType = QT;
                bLoaded = vQT.open(fPath);
            }
        }
    }
}

void VideoOSX::bind(){
    switch (vType) {
        case AVF: vAVF.bind(); break;
        case HAP: vHap.bind(); break;
        case QT:  vQT.bind();  break;
    }
}

void VideoOSX::unbind(){
    switch (vType) {
        case AVF: vAVF.unbind(); break;
        case HAP: vHap.unbind(); break;
        case QT:  vQT.unbind();  break;
    }
}

void VideoOSX::update(){
    switch (vType) {
        case AVF:
            if (vAVF.isLoaded()){
                if (vAVF.isSupported()){
                    bLoaded = true;
                }
                else if (parseForceRenderer(forceVideoRenderer) != AVF){
                    vType = HAP;
                    if (vHap.open(fPath)){
                        bLoaded = true;
                    }
                    else{
                        vType = QT;
                        bLoaded = vQT.open(fPath);
                    }
                    return;
                }
            }
            vAVF.update(); break;
        case HAP: vHap.update(); break;
        case QT:  vQT.update();  break;
    }
    if (getIsMovieDone()) {
        bEnd = true;
        ofNotifyEvent(endEvent,bEnd,this);
    }
}

void VideoOSX::play(){
    switch (vType) {
        case AVF: vAVF.play(); break;
        case HAP: vHap.play(); break;
        case QT:  vQT.play();  break;
    }
}

void VideoOSX::stop(){
    switch (vType) {
        case AVF: vAVF.stop(); break;
        case HAP: vHap.stop(); break;
        case QT:  vQT.stop();  break;
    }
}

void VideoOSX::close(){
    switch (vType) {
        case AVF: vAVF.close(); break;
        case HAP: vHap.close(); break;
        case QT:  vQT.close();  break;
    }
}

void VideoOSX::seek(float f){
    switch (vType) {
        case AVF: vAVF.seek(f); break;
        case HAP: vHap.seek(f); break;
        case QT:  vQT.seek(f);  break;
    }
}

bool VideoOSX::isPlaying(){
    bool val;
    switch (vType) {
        case AVF: val = vAVF.isPlaying(); break;
        case HAP: val = vHap.isPlaying(); break;
        case QT:  val = vQT.isPlaying();  break;
    }
    return val;
}

void VideoOSX::setLoop(bool lp){
    bLoop = lp;
    vAVF.setLoop(lp);
    vHap.setLoop(lp);
    vQT.setLoop(lp);
}

float VideoOSX::getPosition(){
    float val;
    switch (vType) {
        case AVF: val = vAVF.getPosition(); break;
        case HAP: val = vHap.getPosition(); break;
        case QT:  val = vQT.getPosition();  break;
    }
    return val;
}

float VideoOSX::getDuration(){
    float val;
    switch (vType) {
        case AVF: val = vAVF.getDuration(); break;
        case HAP: val = vHap.getDuration(); break;
        case QT:  val = vQT.getDuration();  break;
    }
    return val;
}

bool VideoOSX::getIsMovieDone(){
    bool val;
    switch (vType) {
        case AVF: val = vAVF.getIsMovieDone(); break;
        case HAP: val = vHap.getIsMovieDone(); break;
        case QT:  val = vQT.getIsMovieDone();  break;
    }
    return val;
}

void VideoOSX::setVolume(float v){
    switch (vType) {
        case AVF: vAVF.setVolume(v); break;
        case HAP: vHap.setVolume(v); break;
        case QT:  vQT.setVolume(v);  break;
    }
}

bool VideoOSX::isLoaded(){
    return bLoaded;
}

VideoOSX::VideoTypes VideoOSX::parseForceRenderer(string renderer){
    VideoTypes type;
    renderer = ofToLower(renderer);
    if (renderer == "avf")      type = AVF;
    else if (renderer == "hap") type = HAP;
    else                        type = QT;
    return type;
}