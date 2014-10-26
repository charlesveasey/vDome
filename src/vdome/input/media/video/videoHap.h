#pragma once
#include "ofxHapPlayer.h"
namespace vd{

class VideoHap{
    
public:
    VideoHap();
    
    bool open(string filepath);
	void threadedFunction();
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
    float getWidth();
    float getHeight();

    ofxHapPlayer player;
    ofShader shader;

private:
    ofLoopType loopT;
    bool bEnd;
	bool bHapQ;

};
}