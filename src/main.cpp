#include "ofMain.h"
#include "vdome.h"
#include "ofGlProgrammableRenderer.h"

int main( ){
	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofSetupOpenGL(2048,768, OF_WINDOW);
	ofRunApp(new vdome());
}
