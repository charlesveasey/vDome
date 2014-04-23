#include "ofMain.h"

#ifdef TARGET_OSX
	#include "ofxSyphon.h"
	#include "ofxHapPlayer.h"
    #include "ofxAVFVideoPlayer.h"
    #include "ofxLibav.h"
#endif
#ifdef TARGET_WIN32
	#include "ofDirectShowPlayer.h"
#endif

#include "ofxMediaType.h"

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
    void dragEvent(ofDragInfo dragInfo);

    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);

    void update();

    void bind();
    void unbind();

    int source;
    enum sources {MEDIA, CAPTURE, SYPHON};
	int maxSource;
    string file;

    ofTexture texture;

    int frameRate;

    ofImage image;
    ofVideoPlayer video;
    ofVideoGrabber capture;

    int resolution;

    enum vRendererTypes {AVF, QT, HAP, X};
    int vRenderer;
    
    bool isVideo;
    
    #ifdef TARGET_OSX
        ofxHapPlayer hap;
        ofxSyphonClient syphon;
        ofxAVFVideoPlayer avf;
        ofQTKitPlayer qt;
    #endif
    
    // mime type
    string mediaType;
    MediaTypeMap::SharedPtr mediaTypeMap;
    Poco::File pFile;
    ofFile oFile;
    void parseFileType(string filepath);
    
    // codec
    #ifdef TARGET_OSX
        AVProbe probe;
        enum codecs {CODEC_AV1, CODEC_HAP};
        void parseVideoCodec(string filepath);
    #endif
};

}
