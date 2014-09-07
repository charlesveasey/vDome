#pragma once
#include "ofxWMFVideoPlayer.h"
namespace vd{

class VideoWMF{
    
public:
    VideoWMF();
    
    bool open(string filepath);
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
    
	ofxWMFVideoPlayer player;

private:
    bool bLoop;
	bool bLoaded;
	bool bSupported;
	bool bEnded;
	void videoLoaded(bool &success);

};
}