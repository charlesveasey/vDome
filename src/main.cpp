#include "ofMain.h"
#include "vdome.h"
//#include "ofGlProgrammableRenderer.h"

#include "../../../openFrameworks/gl/ofGLProgrammableRenderer.cpp"

int main( ){
	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofSetupOpenGL(1920,1080, OF_WINDOW);
	ofRunApp(new vd::vdome());
}
