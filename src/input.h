#pragma once
#include "ofMain.h"
#include "ofxMediaType.h"
#include "ofPBO.h"
#include "ofxM3U.h"
#include "render.h"

#ifdef TARGET_OSX
#include "ofxSyphon.h"
#include "ofxHapPlayer.h"
#include "ofxAVFVideoPlayer.h"
#endif

#ifdef TARGET_WIN32
#include "ofDirectShowPlayer.h"
#include "ofxWMFVideoPlayer.h"
#include "ofGstVideoPlayer.h"
#endif
using namespace ofx::Media;

namespace vd {

class Input {

public:
    Input();
    void setup();

    void play();
    void toggle();
    void stop();
    void close();
    void prev();
    void next();
    void seek(float f);
    void nextFrame();
    void prevFrame();
    
    bool getLoop();
    void setLoop(bool b);

    string getSource();
    void setSource(string s);

    string getFile();
    void setFile(string s);

    float getPosition();
    
    void update();
    
    void bind();
    void unbind();
    
	void openFileDialog();
    
    void keyPressed(int key);
    void dragEvent(ofDragInfo dragInfo);
    
    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);

	bool isPlaying();
    
    ofTexture texture;
    ofTexture *tex;
    ofTexture *textures[2];

    
    int editMode;
    enum editModes{NONE, SOURCE, LOOP};

    int source;
    enum sources {MEDIA, CAPTURE, SYPHON, GRID, BLACK, WHITE, GREY};
	int maxSource;
    
    string file;

    bool isVideo;
    bool usePbo;
    int frameRate;
    int resolution;
    float imageDuration;
    
    int vRenderer;
    enum vRendererTypes {AVF, QT2, HAP, QT, WMF, DS, GST};
    
    
private:
    void fillTexture(ofColor color);
    void newFrame(ofPixels & pixels);

    void getPrevFile();
    void getNextFile();
    void parsePlaylist(string filepath);
    
    void startTimer();
    void stopTimer();
    void evalTimer();
    
    
    ofImage image;
    ofVideoPlayer video;
    ofVideoGrabber capture;
    ofPBO pbo;
    
#ifdef TARGET_OSX
    ofxHapPlayer hap;
    ofxSyphonClient syphon;
    ofxAVFVideoPlayer avf;
    ofQTKitPlayer qt;
#endif
    
#ifdef TARGET_WIN32
    ofxWMFVideoPlayer wmf;
#endif
    
    bool loop;
    string filename;
    bool nFrame;
    bool updateTexture;

    ofxM3U m3u;
    string playlist; // FIXME
    vector<string> fileList;
    int fileIndex;
    
    // mime type
    string mediaType;
    MediaTypeMap * mediaTypeMap;
    Poco::File pFile;
    ofFile oFile;
    void parseFileType(string filepath);
    void openMediaFile(vector<string> files);
    
    float startTime;
    float endTime;
    bool timerRunning;
    
    int currentTexture;
    bool firstTexture;
    bool swapTexture;
};
    

}
