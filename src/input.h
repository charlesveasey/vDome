#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxHapPlayer.h"
#include "render.h"

class Input {
	
public:
 
    void init();
    void setup();
    
    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);
    
    void update();
   
    void bind();
    void unbind();

    int mode;
    
    ofTexture texture;
    Render render;
    
    ofImage image;
    ofVideoPlayer video;
    ofxHapPlayer hap;
    ofxSyphonClient syphon;
    ofVideoGrabber capture;
    
};

