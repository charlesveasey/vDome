#pragma once
#include "videoWMF.h"
#include "../video.h"
namespace vd{

class VideoWin {
    
public:
    VideoWin();
    
    void	open(string filepath);
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
    void	setVolume(float v);
	bool	isLoaded();
    float	getWidth();
    float	getHeight();

	void	bind();
	void	unbind();

	ofPixels&       getPixels();
  
    enum VideoTypes	{WMF,DS};
    string			forceVideoRenderer;
    ofEvent<bool>	endEvent;

private:
    VideoTypes	parseForceRenderer(string renderer);
    VideoTypes	vType;

    VideoWMF	vWMF;
    Video		vDS;
    
    bool		bLoop;
    bool		bEnd;
    string		fPath;
	bool		bLoaded;
};
}