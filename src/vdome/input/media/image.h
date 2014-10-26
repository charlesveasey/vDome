#pragma once
#include "ofMain.h"
namespace vd{

class Image {
    
public:
    Image();
    void open(string filepath);
    void update();
    void bind();
    void unbind();
    void play();
    void stop();
    void close();
    void setUseTimer(bool uTimer);
    void setTimerDuration(float dur);
	bool isLoaded();
    float getWidth();
    float getHeight();

    ofEvent<bool> timerEvent;
    ofImage img;

private:
    void startTimer();
    void stopTimer();
    void evalTimer();
    
    float sTime;
    float eTime;
    bool bUseTimer;
    bool bTimerRunning;
    float timerDuration;
	bool bLoaded;

};
}