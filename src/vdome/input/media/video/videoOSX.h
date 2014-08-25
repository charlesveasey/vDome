#pragma once
#include "videoQT.h"
#include "videoAVF.h"
#include "videoHap.h"

namespace vd{

class VideoOSX {
    
public:
    VideoOSX();
    
    void open(string filepath);
    void update();
    void bind();
    void unbind();
    void play();
    void stop();
    void close();
    void seek(float f);
    bool isPlaying();
    void setLoop(bool lp);
    float getPosition();
    float getDuration();
    bool getIsMovieDone();
    void setVolume(float v);
    bool isLoaded();
  
    enum VideoTypes{QT,AVF,HAP};
    string forceVideoRenderer;
    ofEvent<bool> endEvent;
    VideoAVF vAVF;

private:
    VideoTypes parseForceRenderer(string renderer);

    VideoTypes vType;
    VideoQT vQT;
    VideoHap vHap;
    
    bool bLoop;
    bool bEnd;
    string fPath;
    bool bLoaded;
    bool bAVF;
};
}