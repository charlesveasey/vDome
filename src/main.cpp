#include "ofMain.h"
#include "vdome.h"
#include "ofxMultiGLFWWindow.h"
#include "../../../openFrameworks/gl/ofGLProgrammableRenderer.cpp"

int main( ){
    ofSetLogLevel(OF_LOG_SILENT);
	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);

    ofxMultiGLFWWindow glfw;
    glfw.setOpenGLVersion(3,2); // must be set
    glfw.windowCount = 1;
    
    ofSetupOpenGL(&glfw, 1920,1080, OF_WINDOW);
	ofRunApp(new vd::vdome());
}
