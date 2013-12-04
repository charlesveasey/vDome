#include "ofMain.h"
#ifdef TARGET_OSX
	#include "ofxSyphon.h"
	#include "ofxHapPlayer.h"
#endif

#include "render.h"

class Input {
	
public:
 
    void init();
    void setup();
    void stop();
    void close();
    
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
	//ofxHapPlayer hap;
	//ofxSyphonClient syphon;
    ofVideoGrabber capture;
    
};

