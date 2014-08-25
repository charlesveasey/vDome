#ifdef TARGET_LINUX
#pragma once
#include "ofPBO.h"
#include "ofVideoPlayer.h"
namespace vd{
class VideoGST : public ofThread{

public:
    VideoGST();

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
    void setUsePbo(bool uPbo);
    void setLoop(bool lp);
    float getPosition();
    float getDuration();
    bool getIsMovieDone();
    void setVolume(float v);

    ofVideoPlayer player;
    ofTexture texture;
    ofPixels pixels;
    ofPBO pbo;

private:
    bool bPbo;
    ofLoopType loopT;
};
}
#endif