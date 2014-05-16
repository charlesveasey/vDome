#include "ofMain.h"
#include "vdome.h"
#include "ofAppGLFWWindow.h"
#include "../../../openFrameworks/gl/ofGLProgrammableRenderer.cpp"

int main( ){
    ofSetLogLevel(OF_LOG_SILENT);
	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    ofAppGLFWWindow win;
    ofSetupOpenGL(&win, 1920,1080, OF_WINDOW);
	ofRunApp(new vd::vdome());
}
