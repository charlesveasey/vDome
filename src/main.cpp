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

<<<<<<< HEAD
    #ifdef TARGET_WIN32
		ofxWinWindow win;
	#else
		ofAppGLFWWindow win;
	#endif

	//win.setMultiDisplayFullscreen(false);
    ofSetupOpenGL(&win, 1680,1080, OF_WINDOW);
=======
    ofxMultiGLFWWindow glfw;
    glfw.setOpenGLVersion(3,2); // must be set
    glfw.windowCount = 1;
    
    ofSetupOpenGL(&glfw, 1920,1080, OF_WINDOW);
>>>>>>> d-tmp
	ofRunApp(new vd::vdome());
}
