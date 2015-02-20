#include "ofMain.h"
#include "vdome.h"
#include "ofxMultiGLFWWindow.h"
#include "ofGLProgrammableRenderer.cpp"

int main( ){

	#ifdef VDOME_DEBUG
	    ofSetLogLevel(OF_LOG_VERBOSE);
	#else
		ofSetLogLevel(OF_LOG_SILENT);
	#endif

	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    
	ofxMultiGLFWWindow win;
    win.setOpenGLVersion(4,1);
    win.windowCount = 1;
	win.hideBorder();

    ofSetupOpenGL(&win, 1680,1080, OF_WINDOW);
	ofRunApp(new vd::vdome());
}
