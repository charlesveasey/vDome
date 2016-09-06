#pragma once
#include "ofxWMFVideoPlayer.h"
namespace vd{

class VideoWMF{
    
public:
    VideoWMF();
    
    bool	open(string filepath);
    void	update();
    void	play();
    void	stop();
    void	close();
    void	seek(float f);
    bool	isPlaying();
    void	setLoop(bool lp);
    float	getPosition();
    float	getDuration();
    bool	getIsMovieDone();
	bool	isSupported();
	bool	isLoaded();
    void	setVolume(float v);
	float	getWidth();
    float	getHeight();	
	void	bind();
	void	unbind();
	
	ofPixels&		getPixels();
	    
private:
	void	videoLoaded(bool &success);
    
	bool	bLoop;
	bool	bLoaded;
	bool	bSupported;
	bool	bEnded;
	bool	markEnd;
	float	positionRequest;
	int		positionRequestFrameCnt;
	float	storePositionFix;
	int		storePositionFrameCnt;
	float	seekPositionStore;
	int		seekPositionFrameCnt;
	bool	bPaused;

	ofxWMFVideoPlayer player;

};
}