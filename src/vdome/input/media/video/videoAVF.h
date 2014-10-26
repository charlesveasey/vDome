#pragma once
#include "ofxAVFVideoPlayer.h"
namespace vd{

class VideoAVF {
    
public:
    VideoAVF();
    
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
    bool isSupported();
    bool isLoaded();
    void setVolume(float v);
    float getWidth();
    float getHeight();
    
    ofxAVFVideoPlayer player;

private:
    bool videoLoaded(bool &success);
    
    bool bLoaded;
    bool bSupported;
    ofLoopType loopT;
    ofTexture blank;
    ofPixels  pixels;
    int fCnt;
    bool playing;
    float pos;
};
}