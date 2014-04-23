#include "ofMain.h"

#ifdef TARGET_OSX
	#include "ofxSyphon.h"
	#include "ofxHapPlayer.h"
    #include "ofxAVFVideoPlayer.h"
#endif
#ifdef TARGET_WIN32
	#include "ofDirectShowPlayer.h"
#endif

#include "ofxMediaType.h"
#include "ofxLibav.h"
#include "render.h"
using namespace ofx::Media;

namespace vd {

class Input {

public:
    Input();
    void setup();
    void stop();
    void close();

    void keyPressed(int key);

    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);

    void update();

    void bind();
    void unbind();

    int source;
	int maxSource;
    string file;

    ofTexture texture;

    int frameRate;

    ofImage image;
    ofVideoPlayer video;
    ofVideoGrabber capture;

    int resolution;

    enum vRendererTypes {AVF, QT, HAP};
    int vRenderer;
    
    bool isVideo;
    
    #ifdef TARGET_OSX
        ofxHapPlayer hap;
        ofxSyphonClient syphon;
        ofxAVFVideoPlayer avf;
        ofQTKitPlayer qt;
    #endif
    
    // file metadata
    string mediaType;
    MediaTypeMap::SharedPtr mediaTypeMap;
    Poco::File pFile;
    ofFile oFile;
    
    AVProbe probe;
    enum codecs {CODEC_AV1, CODEC_HAP};

    void parseVideoCodec(string filepath);
};

}
