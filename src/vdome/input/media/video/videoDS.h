#pragma once
#include "ofPBO.h"
#include "ofVideoPlayer.h"
#include "ofDirectShowPlayer.h"
namespace vd{

class VideoDS {
    
public:
    VideoDS();
    
    bool open(string filepath);
    void update();
    void bind();
    void unbind();
    void play();
    void stop();
    void close();
    void seek(float f);
    bool isPlaying();
    void setUsePbo(bool uPbo);
    void setLoop(bool lp);
    float getPosition();
    float getDuration();
    bool getIsMovieDone();
    void setVolume(float v);
    
	ofVideoPlayer player;
    ofPixels pixels;
    ofPBO pbo;

private:
    bool bPbo;
    ofLoopType loopT;
};
}