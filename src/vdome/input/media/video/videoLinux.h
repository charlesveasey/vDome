#ifdef TARGET_LINUX

#pragma once
#include "video/videoGST.h"
namespace vd{

class VideoLinux {

public:
    VideoLinux();

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

    ofEvent<bool> endEvent;
    string forceVideoRenderer;

private:
    VideoGST vGST;
    bool bEnd;
    bool bLoaded;
};
}
#endif
