#pragma once
#include "image.h"

#ifdef TARGET_WIN32
#include "videoWin.h"
#else
#include "video.h"
#endif

namespace vd{

class Media{

public:
    Media();

    void    open(string filepath);
    void    update();
    void    play();
    void    stop();
    void    close();
    void    seek(float f);
    
    void    setLoop(bool lp);
    bool    isPlaying();
    float   getPosition();
    float   getDuration();
    bool    getLoop();
    string  getFilepath();
    void    setVolume(float v);
    void    setResolution(int w, int h);
	bool    isLoaded();
    float   getRealWidth();
    float   getRealHeight();

	void	bind();
	void	unbind();
    
    ofPixels&   getPixels();

    enum MediaTypes {IMAGE,VIDEO};
    MediaTypes      mType;

    ofEvent<bool>   endEvent;


private:
    string  parseFile(string filepath);
    void    videoEnd(bool &end);
    bool    isImageFile(string ext);

    int     width;
    int     height;
    bool    bLoop;
    string  fpath;
    bool    bEnded;
    float   vol;

	Image   image;

#ifdef TARGET_WIN32
	VideoWin	video;
#else
	Video	video;
#endif
    
    string ImageTypes[29] =
        {"BMP","DDS","EXR","GIF","HDR","ICO","IFF","JBIG","JNG", "JPG",
        "JPEG","JIF","KOALA","MNG","PCX","PBM","PFM","PNG","PICT",
        "PSD","RAW","RAS","SGI","TARGA","TIFF","WBMP","WEBP","XBM","XPM"};
};
}
