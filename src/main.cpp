#include "ofMain.h"
#include "vdome.h"
#include "ofxMultiGLFWWindow.h"
#include "../../../openFrameworks/gl/ofGLProgrammableRenderer.cpp"

#ifdef TARGET_WIN32
	#include "ofxWinWindow.h"
#endif

int main( ){
    ofSetLogLevel(OF_LOG_SILENT);
	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    
    #ifdef TARGET_WIN32
		ofxWinWindow win;
	#else
		ofxMultiGLFWWindow win;
        win.setOpenGLVersion(3,2); // must be set
        win.windowCount = 1;
	#endif

    ofSetupOpenGL(&win, 1680,1080, OF_WINDOW);
	ofRunApp(new vd::vdome());
}
