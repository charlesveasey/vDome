#pragma once
#include "ofMain.h"
#include "render.h"
#include "socket.h"
#include "media.h"
#include "capture.h"
#include "color.h"
#include "model.h"
#include "spoutR.h"

#ifdef TARGET_OSX
#include "ofxSyphon.h"
#endif

namespace vd {
class Socket;
    
class Input {

public:
    Input();
    void setup();
    void update();
    void bind();
    void unbind();
    
    void play();
    void stop();
    void close();
    void previous();
    void next();
    void seek(float f);
	bool isPlaying();
    void setLoop(bool b);
    bool getLoop();
    string getSource();
    void setSource(string s);
    float getPosition();
    float getDuration();
    void setResolution(int r);
    void setSlide(int s);
    string getFilepath();
    void setVolume(float v);
    
    void openFile(string filepath);
    void setFile(string filepath);
    void openFileDialog();
    void dragEvent(ofDragInfo dragInfo);
    
    void keyPressed(int key);

    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);
    
    int editMode;
    enum editModes{NONE, SOURCE, LOOP};

    int source;
    enum sources {MEDIA, CAPTURE, SYPHON, SPOUT, GRID, BLACK, WHITE, GREY};
	int maxSource;

    bool isVideo;
    int framerate;
    int resolution;
    float slide;
    
    int renderer;
    enum rendererT {AVF, QT2, HAP, QT, WMF, DS, GST};
    
    Socket *socket;
	Model *model;

private:
    void mediaEnd(bool &end);
    
    Media media;
    Capture capture;
    Color color;

#ifdef TARGET_WIN32
	SpoutR spout;
#endif

#ifdef TARGET_OSX
    ofxSyphonClient syphon;
#endif
    
    vector<string> files;
    bool loop;
    int lastSource;
    bool durationSent;
    bool endSent;
   
};
    
}
