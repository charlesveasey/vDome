#pragma once
#include "ofMain.h"
#include "socket.h"
#include "media.h"
#include "capture.h"
#include "color.h"
#include "model.h"

#ifdef TARGET_WIN32
#include "spoutR.h"
#endif

#ifdef TARGET_OSX
#include "ofxSyphon.h"
#endif

namespace vd {
class Socket;
    
class Input {

public:
    Input();
    
    void    setup();
    void    update();
    
    void    play();
    void    stop();
    void    close();
    void    seek(float f);
	bool    isPlaying();
    
    void    loadXML(ofXml &xml);
    void    saveXML(ofXml &xml);
    
	void    setLoop(bool b);
    bool    getLoop();
    
	string  getSource();
	int     getSourceInt();
    void    setSourceInt(int i);    
    int     convertSourceString(string s);

    void    setFormatInt(int i);
    void    setFormat();
	int     getFormatInt();
    int     convertFormatString(string s);

    float   getPosition();
    float   getDuration();
   
	void    setResolution(int r);
    int    getResolution();
    
    string  getFilepath();

    void    setVolume(float v);
	void    setColor(int r, int g, int b);
    
    void    openFile(string file);
    void    setFile(string file);
    void    setFramerate(int frate);
	
	void	bind();
	void	unbind();
    
    ofPixels&   getPixels();
    
    int     source;
    enum    sources {MEDIA, CAPTURE, SYPHON, SPOUT, GRID, BLACK, WHITE, GREY, COLOR};
    
    int     format;
    enum    format {DOMEMASTER, HD};
    
    Media   media;
    Capture capture;
    Color   color;
    Socket  *socket;

    float   ratio;
    bool    durationSent;
    
private:
    void    mediaEnd(bool &end);
    
    string  filepath;
    bool    loop;
    bool    endSent;
    int     framerate;
    bool    isVideo;
    int     resolution;

	ofColor cColor;
};
    
}
