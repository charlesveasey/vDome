#include "ofMain.h"
#include "vdome.h"
#include "ofAppGLFWWindow.h"
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
		ofAppGLFWWindow win;
	#endif

	//win.setMultiDisplayFullscreen(false);
    ofSetupOpenGL(&win, 1680,1080, OF_WINDOW);
	ofRunApp(new vd::vdome());
}
