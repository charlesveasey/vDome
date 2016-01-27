#pragma once
#include "ofVideoPlayer.h"
namespace vd {

	class Video {

	public:
		Video();

		bool    open(string filepath);
		void    update();

		void    play();
		void    stop();
		void    close();
		void    seek(float f);

		bool    isPlaying();
		bool    isLoaded();
		void    setLoop(bool lp);
		float   getPosition();
		float   getDuration();
		bool    getIsMovieDone();
		void    setVolume(float v);
		float   getWidth();
		float   getHeight();

		ofVideoPlayer   player;
		ofPixels        pixels;
		ofEvent<bool>   endEvent;
		ofPixels&       getPixels();
		ofTexture&      getTexture();

	private:
		bool        bEnded;
		ofLoopType  loopT;
	};
}