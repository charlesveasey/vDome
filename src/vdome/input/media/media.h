#pragma once
#include "image.h"
#include "capture.h"
#include "ofxMediaType.h"
#include "MediaTypeMap.h"
#include "ofMain.h"
#include "ofxM3U.h"

#ifdef TARGET_OSX
#include "videoOSX.h"
#endif
#ifdef TARGET_WIN32
#include "videoWin.h"
#endif
#ifdef TARGET_LINUX
#include "videoLinux.h"
#endif

using namespace ofx::Media;

namespace vd{

class Media{
    
public:
    Media();
    
    void open(string filepath);
    void open(vector<string> filelist);
    void update();
    void bind();
    void unbind();
    void play();
    void stop();
    void close();
    void seek(float f);
    void previous();
    void next();
    void setLoop(bool lp);
    bool isPlaying();
    float getPosition();
    float getDuration();
    bool getLoop();
    string getFilepath();
    void setVolume(float v);
    void setResolution(int w, int h);
    void setSlideshow(bool enable, int duration);
	bool isLoaded();
    
    enum MediaTypes {IMAGE,VIDEO};
    MediaTypes mType;

    Image image;
    string forceVideoRenderer;
    ofEvent<bool> endEvent;
   
#ifdef TARGET_OSX
	VideoOSX video;
#endif
#ifdef TARGET_WIN32
	VideoWin video;
#endif
#ifdef TARGET_LINUX
	VideoLinux video;
#endif
    
private:
    string parseFile(string filepath);
    void parsePlaylist(string filepath);
    void slideEnd(bool &end);
    void videoEnd(bool &end);
    
    int width;
    int height;
    vector<string> fList;
    int fIndex;
    bool bLoop;
    bool bSlideshow;
    ofxM3U m3u;
    MediaTypeMap * mediaTypeMap;
    string fpath;
    bool bEnded;
};
}