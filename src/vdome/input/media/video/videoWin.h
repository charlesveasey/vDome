#pragma once
#include "video/videoWMF.h"
#include "video/videoHap.h"
#include "video/videoDS.h"
namespace vd{

class VideoWin {
    
public:
    VideoWin();
    
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
  
    enum VideoTypes{WMF,HAP,DS};
    string forceVideoRenderer;
    ofEvent<bool> endEvent;

private:
    VideoTypes parseForceRenderer(string renderer);
    VideoTypes vType;

    VideoWMF vWMF;
    VideoHap vHap;
    VideoDS vDS;
    
    bool bLoop;
    bool bEnd;
    string fPath;
	bool bLoaded;
};
}