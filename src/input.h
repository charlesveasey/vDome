#include "ofMain.h"

#ifdef TARGET_OSX
	#include "ofxSyphon.h"
	#include "ofxHapPlayer.h"
    #include "ofxAVFVideoPlayer.h"
    #include "ofxLibav.h"
#endif
#ifdef TARGET_WIN32
	#include "ofDirectShowPlayer.h"
	#include "ofxWMFVideoPlayer.h"
    #include "ofGstVideoPlayer.h"
#endif

#include "ofxMediaType.h"
#include "ofPBO.h"

#include "render.h"
using namespace ofx::Media;

namespace vd {

class Input {

public:
    Input();
    void setup();
    void stop();
    void close();
    void togglePause();

    void keyPressed(int key);
    void dragEvent(ofDragInfo dragInfo);

    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);

    void update();

    void bind();
    void unbind();
    
	void openFileDialog();

    int source;
    enum sources {MEDIA, CAPTURE, SYPHON, GRID, BLACK, WHITE, GREY};
	int maxSource;
    string file;
    string filename;

    ofTexture texture;
	ofPBO pbo;
	bool usePbo;
	bool updateTexture;
	void newFrame(ofPixels & pixels);
	bool nFrame;

    int frameRate;

    ofImage image;
    ofVideoPlayer video;
    ofVideoGrabber capture;

    int resolution;
    bool loop;

    enum vRendererTypes {AVF, QT2, HAP, QT, WMF, DS, GST};
    int vRenderer;
    
    bool isVideo;
    
    #ifdef TARGET_OSX
        ofxHapPlayer hap;
        ofxSyphonClient syphon;
        ofxAVFVideoPlayer avf;
        ofQTKitPlayer qt;
    #endif
    
	#ifdef TARGET_WIN32
		ofxWMFVideoPlayer wmf;
	#endif

    // mime type
    string mediaType;
    MediaTypeMap * mediaTypeMap;
    Poco::File pFile;
    ofFile oFile;
    void parseFileType(string filepath);
    
    // codec
    #ifdef TARGET_OSX
        AVProbe probe;
        enum codecs {CODEC_AV1, CODEC_HAP};
        void parseVideoCodec(string filepath);
    #endif
    
    void fillTexture(ofColor color);
};

}
